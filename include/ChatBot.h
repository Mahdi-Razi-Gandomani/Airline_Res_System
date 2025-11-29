#ifndef CHATBOT_H
#define CHATBOT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <onnxruntime_cxx_api.h>
#include "ReservationSystem.h"

using namespace std;
using namespace Ort;



struct Document {
    string id;
    string content;
    string category;
    vector<float> embedding;
};

struct RetedConts {
    vector<Document> documents;
    vector<Flight> flights;
    vector<shared_ptr<Ticket>> tickets;
    string structData;
};


class ChatBot : public QObject {
    Q_OBJECT


    
public:
    explicit ChatBot(ReservationSystem* system, QObject* parent = nullptr);
    ~ChatBot();

    void process(const QString& userQuery);

signals:
    void responseReady(const QString& response);
    void errored(const QString& error);

private:

    ReservationSystem* reservationSystem;
    QNetworkAccessManager* netManager;
    
    // Onnx run
    unique_ptr<Env> ortEnv;
    unique_ptr<Session> ortSess;
    SessionOptions sessOpts;
    AllocatorWithDefaultOptions allocator;
    


    
    // Knowledge base
    vector<Document> knowledgeBase;
    unordered_map<string, vector<float>> embedCache;
    
    // History
    vector<pair<string, string>> convHist;
    string lastQuery;
    static const int maxHist = 5;
    
    // Tokenize
    unordered_map<string, int64_t> vocab;
    unordered_map<int64_t, string> idToToken;
    void loadvocab();
    vector<int64_t> tokenize(const string& text);
    
    
    void initKnowledge();
    void initOnnx();
    void addDoc(const string& content, const string& category);
    

    // Embedding
    vector<float> getEmbedding(const string& text);
    vector<float> onnxInfer(const vector<int64_t>& , const vector<int64_t>&);
    
    // Retrieval
    RetedConts retrieveCont(const string& query);
    vector<Document> semanticSearch(const string& query, int topK);
    vector<Flight> searchFlights(const string& query);
    vector<shared_ptr<Ticket>> searchTickets(const string& query);
    
    

    float cosineSim(const vector<float>& a, const vector<float>& b);
    string detectIntent(const string& query);
    bool isStruct(const string& query);
    vector<string> extractEntities(const string&);
    
    // llm
    void llmGen(const string& query, const RetedConts& context);
    string promptt(const string& query, const RetedConts& context);
    void llmRes(QNetworkReply* reply);
};

#endif