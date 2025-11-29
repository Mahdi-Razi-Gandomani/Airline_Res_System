#include "ChatBot.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <regex>
#include <fstream>
#include <cctype>

using namespace std;
using namespace Ort;

ChatBot::ChatBot(ReservationSystem* system, QObject* parent)
    : QObject(parent), reservationSystem(system) {
    netManager = new QNetworkAccessManager(this);
    
    try {
        initOnnx();
        // qDebug() << "onnx ok";
        
        loadvocab();
        initKnowledge();

        
    } catch (const exception& e) {
        qDebug() << e.what();
    }
}

ChatBot::~ChatBot() {
}


void ChatBot::loadvocab() {
    QString vocabPath = QDir::currentPath() + "/models/vocab.txt";
    
    ifstream file(vocabPath.toStdString());
    if (!file.is_open()) {
        throw runtime_error("vocabulary file not found");
    }
    
    string token;
    int64_t id = 0;
    
    while (getline(file, token)) {
        token.erase(find_if(token.rbegin(), token.rend(), 
                   [](unsigned char ch) { return !isspace(ch); }).base(), token.end());
        
        if (!token.empty()) {
            vocab[token] = id;
            idToToken[id] = token;
            ++id;
        }
    }

    file.close();
    
    // qDebug() << "vocab size" << vocab.size();
}

vector<int64_t> ChatBot::tokenize(const string& text) {
    vector<int64_t> tokenIds;
    
    tokenIds.push_back(101); // cls?
    

    string cleaned = text;
    transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ::tolower);
    vector<string> words;
    string current;
    
    for (char c : cleaned) {
        if (isspace(c) || ispunct(c)) {
            if (!current.empty()) {
                words.push_back(current);
                current.clear();
            }
            if (ispunct(c)) {
                words.push_back(string(1, c));
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        words.push_back(current);
    }
    


    for (const auto& word : words) {
        if (word.length() > 200) {
            // 101?
            tokenIds.push_back(100);
            continue;
        }
        
        size_t start = 0;
        bool wordAdded = false;
        
        while (start < word.length()) {
            size_t end = word.length();
            bool found = false;
            
            while (start < end) {
                string substr = word.substr(start, end - start);
                if (start > 0) {
                    substr = "##" + substr;
                }
                
                auto it = vocab.find(substr);
                if (it != vocab.end()) {
                    tokenIds.push_back(it->second);
                    found = true;
                    wordAdded = true;
                    break;
                }
                --end;
            }
            
            if (!found) {
                tokenIds.push_back(100); 
                break;
            }
            
            start = end;
        }
        if (!wordAdded) {
            tokenIds.push_back(100);
        }
    }
    
    // search again//////
    tokenIds.push_back(102);
    if (tokenIds.size() > 128) {
        tokenIds.resize(128);
        tokenIds[127] = 102;
    } else {
        while (tokenIds.size() < 128) {
            tokenIds.push_back(0);
        }
    }
    
    return tokenIds;
}




void ChatBot::initOnnx() {
    try {
        ortEnv = make_unique<Env>(ORT_LOGGING_LEVEL_WARNING, "ChatBot");
        
        /////////////////////
        // Configs
        sessOpts.SetIntraOpNumThreads(4);
        sessOpts.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
        //////////////////////
        
        QString modelPath = QDir::currentPath() + "/models/sbert_model.onnx";
        
        if (!QFile::exists(modelPath)) {
            throw runtime_error("model not founddd");
        }
        
#ifdef _WIN32
        ortSess = make_unique<Session>(*ortEnv, modelPath.toStdWString().c_str(), sessOpts);
#else
        ortSess = make_unique<Session>(*ortEnv, modelPath.toStdString().c_str(), sessOpts);
#endif
        
        
    } catch (const Exception& e) {
        throw runtime_error(string("ONNX Runtime error: ") + e.what());
    }
}

vector<float> ChatBot::getEmbedding(const string& text) {
    if (embedCache.find(text) != embedCache.end()) {
        return embedCache[text];
    }
    
    try {
        // Tokenize and mask
        vector<int64_t> ids = tokenize(text);
        vector<int64_t> attMask;
        for (auto id : ids) {
            attMask.push_back(id != 0 ? 1: 0);
        }
        
        // Get embedding
        vector<float> embedding = onnxInfer(ids, attMask);
        
 

        embedCache[text] = embedding;
        
        return embedding;
    } catch (const exception& e) {
        qDebug() << e.what();
        return {};
    }
}

vector<float> ChatBot::onnxInfer(const vector<int64_t>& ids, const vector<int64_t>& attMask) {
    try {
        vector<int64_t> inShape = {1, static_cast<int64_t>(ids.size())};
        auto mem = MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

vector<int64_t> inCopy = ids;
vector<int64_t> maskCopy = attMask;
        
        Value inputTensor = Value::CreateTensor<int64_t>(
            mem, inCopy.data(), inCopy.size(), 
            inShape.data(), inShape.size()
        );

        Value maskTensor = Value::CreateTensor<int64_t>(
            mem, maskCopy.data(), maskCopy.size(),
            inShape.data(), inShape.size()
        );
        

        const char* inNames[] = {"input_ids", "attention_mask"};
        const char* outNames[] = {"last_hidden_state"};
        
        vector<Value> inputTensors;
        inputTensors.push_back(std::move(inputTensor));
        inputTensors.push_back(std::move(maskTensor));
        
        auto outt = ortSess->Run(RunOptions{nullptr}, inNames, inputTensors.data(), 2,outNames, 1);
        
        // Extract embeddings
        float* outData = outt[0].GetTensorMutableData<float>();
        auto outShape = outt[0].GetTensorTypeAndShapeInfo().GetShape();
        int seqLen = outShape[1];
        int hiddenSize = outShape[2];
        
        vector<float> embedding(hiddenSize, 0.0f);
        int validTokens = 0;
        for (int i = 0; i < seqLen; ++i) {
            if (attMask[i] == 1) {
                for (int j = 0; j < hiddenSize; ++j) {
                    embedding[j] += outData[i * hiddenSize + j];
                }
                validTokens++;
            }
        }
        

        if (validTokens > 0) {
            for (float& val : embedding) {
                val /= static_cast<float>(validTokens);
            }
        }
        

        // Normalize
        float norm = 0.0f;
        for (float val : embedding) {
            norm += val * val;
        }
        norm = sqrt(norm);
        
        if (norm > 0) {
            for (float& val : embedding) {
                val /= norm;
            }
        }
        
        return embedding;
        
    } catch (const Exception& e) {
        //
        return {};
    }
}



void ChatBot::initKnowledge() {
    addDoc(
        "Baggage Policy: Economy passengers are allowed 1 carry-on bag (max 7kg) and 1 checked bag (max 23kg)."
        "Business class passengers get 2 carry-on bags and 2 checked bags (max 32kg each)."
        "First class passengers get 2 carry-on bags and 3 checked bags (max 32kg each).",
        "policy"
    );
    
    addDoc(
        "Cancellation Policy: Tickets can be cancelled up to 24 hours before departure."
        "If flight is cancelled by airline, full refund is provided."
        "Refunds are processed to wallet.",
        "policy"
    );
    
    addDoc(
        "Check-in Policy: Online check-in opens 24 hours before departure and closes 1 hour before. "
        "Airport check-in counters open 3 hours before international flights and 2 hours before domestic flights. "
        "Gates close 30 minutes before departure. Please arrive early.",
        "policy"
    );
    
    
    addDoc("Meal Options: Economy passengers can add meals for $25, In-flight snacks and beverages are complimentary in all classes.", "policy");
    
    addDoc(
        "Travel Insurance: Optional travel insurance covers trip cancellation, medical emergencies, lost baggage, and flight delays.",
        "policy"
    );
    
    addDoc(
        "Payment Methods: We accept credit cards and wallet payments. "
        "All payments are processed securely. For wallet payments, ensure sufficient balance. "
        "Refunds are issued to the wallet.",
        "policy"
    );
}

void ChatBot::addDoc(const string& content, const string& category) {
    Document doc;
    doc.id = "doc_" + to_string(knowledgeBase.size() + 1);
    doc.content = content;
    doc.category = category;
    
    try {
        doc.embedding = getEmbedding(content);
        knowledgeBase.push_back(doc);
    } catch (const exception& e) {
        qDebug() << e.what();
    }
}

float ChatBot::cosineSim(const vector<float>& a, const vector<float>& b) {
    if (a.size() != b.size()) return 0.0f;
    
    float dot = 0.0f;
    for (size_t i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i];
    }
    
    return dot;
}

vector<Document> ChatBot::semanticSearch(const string& query, int topK) {
    try {
        vector<float> qEmb = getEmbedding(query);
        
        vector<pair<float, Document>> scored;
        for (const auto& doc : knowledgeBase) {
            float sim = cosineSim(qEmb, doc.embedding);
            scored.push_back({sim, doc});
        }
        
        sort(scored.begin(), scored.end(), 
             [](const auto& a, const auto& b) { return a.first > b.first; });
        
        vector<Document> results;
        for (int i = 0; i < min(topK, (int)scored.size()); ++i) {
            results.push_back(scored[i].second);
        }
        
        return results;
        
    } catch (const exception& e) {
        // qDebug() << "semantic phase:     " << e.what();
        return {};
    }
}



void ChatBot::process(const QString& userQuery) {
    string query = userQuery.toStdString();
    lastQuery = query;
    try {
        RetedConts context = retrieveCont(query);
        llmGen(query, context);
        
    } catch (const exception& e) {
        //
    }
}

RetedConts ChatBot::retrieveCont(const string& query) {
    RetedConts context;
    
    try {
        context.documents = semanticSearch(query, 3);
        
        if (isStruct(query)) {
            context.flights = searchFlights(query);
            auto passenger = reservationSystem->getThisPass();
            if (passenger) {
                context.tickets = searchTickets(query);
            }
        }
        
        // Build data string
        stringstream ss;
        
        if (!context.flights.empty()) {
            ss << "\n--- Available Flights ---\n";
            for (const auto& flight : context.flights) {
                ss << "Flight " << flight.getFlightNo() << ": " << flight.getSource() << " → " << flight.getDestination() << ", Departure: " << flight.getdepartTime() << ", Date: " << flight.getdepartDate() 
                    << ", Price: $" << flight.getBaseFare() << ", Available seats: " << flight.getAvailableSeats() << "\n";
            }
        }
        
        if (!context.tickets.empty()) {
            ss << "\n--- Your Bookings ---\n";
            for (const auto& ticket : context.tickets) {
                ss << "Ticket " << ticket->getTicketId() << ": Flight " << ticket->getFlight()->getFlightNo() << ", Seat: " << ticket->getSeatNumber()  << ", Price: $" << ticket->getPrice() << "\n";
            }
        }
        
        auto passenger = reservationSystem->getThisPass();
        if (passenger) {
            ss << "\n--- Your Profile ---\n";
            ss << "Wallet Balance: $" << passenger->getWalletBalance() << "\n";
            ss << "Total Bookings: " << passenger->getBookedTickets().size() << "\n";
        }
        
        context.structData = ss.str();
        
    } catch (const exception& e) {
        // qDebug() << "      retrieveCont:       " << e.what();
    }
    
    return context;
}

vector<string> ChatBot::extractEntities(const string& query) {
    vector<string> entities;
    smatch match;
    regex flightPattern(R"([A-Z]{2}\d{3,4})", regex::icase);
    
    string::const_iterator searchStart(query.cbegin());
    while (regex_search(searchStart, query.cend(), match, flightPattern)) {
        entities.push_back(match[0]);
        searchStart = match.suffix().first;
    }
    
    return entities;
}

vector<Flight> ChatBot::searchFlights(const string& query) {
    auto entities = extractEntities(query);
    
    string source, dest, date;
    smatch match;

    regex flightRegex(R"(flight\s+([A-Z]{2}\d{3,4}))", regex::icase);
    regex routeRegex(R"((\w+)\s+to\s+(\w+))", regex::icase);
    
    
    
    if (regex_search(query, match, flightRegex)) {
        string flightNo = match[1].str();
        transform(flightNo.begin(), flightNo.end(), flightNo.begin(), ::toupper);
        auto flight = reservationSystem->getFlightbyNo(flightNo);
        if (flight) {
            return {*flight};
        }
    }
    
    if (regex_search(query, match, routeRegex)) {
        source = match[1].str();
        dest = match[2].str();
    }
    
    return reservationSystem->searchFlights(source, dest, date);
}

vector<shared_ptr<Ticket>> ChatBot::searchTickets(const string& query) {
    auto passenger = reservationSystem->getThisPass();
    if (!passenger) {
        return {};
    }
    
    auto tickets = passenger->getBookedTickets();
    regex ticketRegex(R"(ticket\s+(\w+))", regex::icase);
    
    smatch match;
    if (regex_search(query, match, ticketRegex)) {
        string ticketId = match[1].str();
        transform(ticketId.begin(), ticketId.end(), ticketId.begin(), ::toupper);
        
        vector<shared_ptr<Ticket>> filtered;
        for (const auto& ticket : tickets) {
            if (ticket->getTicketId().find(ticketId) != string::npos) {
                filtered.push_back(ticket);
            }
        }
        return filtered;
    }
    
    return tickets;
}

string ChatBot::detectIntent(const string& query) {
    try {
        vector<pair<string, string>> intents = {
            {"booking", "book reserve purchase buy flight ticket"},
            {"cancellation", "cancel refund delete remove booking"},
            {"search", "find search look show available flights"},
            {"policy", "policy baggage meal insurance rules regulations"}
        };
        vector<float> queryEmb = getEmbedding(query);
        float maxSim = 0.0f;
        string detectedIntent = "general";
        
        for (const auto& [intent, keywords] : intents) {
            vector<float> intentEmb = getEmbedding(keywords);
            float sim = cosineSim(queryEmb, intentEmb);
            if (sim > maxSim) {
                maxSim = sim;
                detectedIntent = intent;
            }

        }
        
        return detectedIntent;
        
    } catch (const exception& e) {
        return "general";
    }
}

bool ChatBot::isStruct(const string& query) {
    string lower = query;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    
    vector<string> sk = {"flight", "ticket", "booking", "search", "show", "find", "my", "wallet", "balance", "profile"};
    
    for (const auto& keyword : sk) {
        if (lower.find(keyword) != string::npos) {
            return true;
        }
    }
    
    return false;
}

// LLM phase

string ChatBot::promptt(const string& query, const RetedConts& context) {
    stringstream prompt;
    
    prompt << "You are a helpful airline reservation assistant. ";
    prompt << "Answer accurately and concisely ONLY based on the provided context. Keep your answers as short as possible\n\n";
    
    if (!convHist.empty()) {
        prompt << "--- Previous Conversation ---\n";
        for (const auto& [userMsg, assistantMsg] : convHist) {
            prompt << "User: " << userMsg << "\n";
            prompt << "Assistant: " << assistantMsg << "\n\n";
        }
        prompt << "--- End of Previous Conversation ---\n\n";
    }
    
    if (!context.documents.empty()) {
        prompt << "--- Airline Policies ---\n";
        for (const auto& doc : context.documents) {
            prompt << doc.content << "\n\n";
        }
    }
    
    if (!context.structData.empty()) {
        prompt << context.structData << "\n";
    }
    
    prompt << "--- Current User Message ---\n";
    prompt << "User: " << query << "\n\n";
    
    prompt << "INSTRUCTIONS:\n";
    prompt << "- Be conversational and remember context\n";
    prompt << "- Reference previous messages when relevant\n";
    prompt << "- Provide accurate information from the context\n\n";
    
    prompt << "Assistant:";
    
    return prompt.str();
}

void ChatBot::llmGen(const string& query, const RetedConts& context) {
    QString prompt = QString::fromStdString(promptt(query, context));
    QUrl url("https://api.groq.com/openai/v1/chat/completions");
    QNetworkRequest request(url);
    
    QByteArray apiKey = QByteArray("your-groq-api-key-here");

    request.setRawHeader("Authorization", "Bearer " + apiKey);
    request.setRawHeader("Content-Type", "application/json");
    
    QJsonObject user;
    user["role"] = "user";
    user["content"] = prompt;
    
    QJsonArray messages;
    messages.append(user);
    
    QJsonObject payload;
    payload["model"] = "llama-3.3-70b-versatile";
    payload["messages"] = messages;
    payload["max_tokens"] = 900;
    
    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();
    
    QNetworkReply* reply = netManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        llmRes(reply);
    });
}



void ChatBot::llmRes(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(bytes);
        QJsonObject obj = doc.object();
        
        if (obj.contains("choices")) {
            QJsonObject choice = obj["choices"].toArray()[0].toObject();
            QJsonObject msg = choice["message"].toObject();
            QString content = msg["content"].toString();
            
            convHist.push_back({lastQuery, content.toStdString()});
            while (convHist.size() > maxHist) {
                convHist.erase(convHist.begin());
            }
            
            emit responseReady(content);
        } else {
            emit errored("Invalid response from LLM");
        }
    } else {
        emit errored(reply->errorString());
    }
    reply->deleteLater();
}