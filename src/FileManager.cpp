#include "FileManager.h"
#include "Ticket.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

using namespace std;

FileManager::FileManager() {}

bool FileManager::fileExists(const string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

void FileManager::initDataFiles() {

    if (!fileExists(FLIGHTS_FILE)) {
        ofstream file(FLIGHTS_FILE);
        file.close();
    }
    if (!fileExists(PASSENGERS_FILE)) {
        ofstream file(PASSENGERS_FILE);
        file.close();
    }
    if (!fileExists(ADMINS_FILE)) {
        ofstream file(ADMINS_FILE);
        file.close();
    }
}

string FileManager::scToString(SeatClass cls) {
    switch (cls) {
        case SeatClass::ECONOMY: return "ECONOMY";
        case SeatClass::BUSINESS: return "BUSINESS";
        case SeatClass::FIRST_CLASS: return "FIRST_CLASS";
        default: return "ECONOMY";
    }
}

SeatClass FileManager::stringTosc(const string& str) {
    if (str == "BUSINESS") return SeatClass::BUSINESS;
    if (str == "FIRST_CLASS") return SeatClass::FIRST_CLASS;
    return SeatClass::ECONOMY;
}

string FileManager::fsToString(FlightStatus status) {
    switch (status) {
        case FlightStatus::SCHEDULED: return "SCHEDULED";
        case FlightStatus::DEPARTED: return "DEPARTED";
        case FlightStatus::ARRIVED: return "ARRIVED";
        case FlightStatus::DELAYED: return "DELAYED";
        case FlightStatus::CANCELLED: return "CANCELLED";
        default: return "SCHEDULED";
    }
}

FlightStatus FileManager::stringTofs(const string& str) {
    if (str == "DEPARTED") return FlightStatus::DEPARTED;
    if (str == "ARRIVED") return FlightStatus::ARRIVED;
    if (str == "DELAYED") return FlightStatus::DELAYED;
    if (str == "CANCELLED") return FlightStatus::CANCELLED;
    return FlightStatus::SCHEDULED;
}

string FileManager::pmToString(PaymentMethod pm) {
    switch (pm) {
        case PaymentMethod::CREDIT_CARD: return "CREDIT_CARD";
        case PaymentMethod::WALLET: return "WALLET";
        default: return "CREDIT_CARD";
    }
}

PaymentMethod FileManager::stringTopm(const string& str) {
    if (str == "WALLET") return PaymentMethod::WALLET;
    return PaymentMethod::CREDIT_CARD;
}




vector<Flight> FileManager::loadFlights() {
    vector<Flight> flights;
    ifstream file(FLIGHTS_FILE);
    
    if (!file.is_open()) {
        cerr << "Unable to open flights file.\n";
        return flights;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string flightNo, airline, source, dest, depTime, arrTime, depDate, statusStr;
        int totalSeats, availableSeats;
        double baseFare;
        
        getline(ss, flightNo, ',');
        getline(ss, airline, ',');
        getline(ss, source, ',');
        getline(ss, dest, ',');
        getline(ss, depTime, ',');
        getline(ss, arrTime, ',');
        getline(ss, depDate, ',');
        getline(ss, statusStr, ',');
        ss >> totalSeats;
        ss.ignore();
        ss >> availableSeats;
        ss.ignore();
        ss >> baseFare;
        
        Flight flight(flightNo, airline, source, dest, depTime, arrTime, depDate, totalSeats, baseFare);
        flight.setStatus(stringTofs(statusStr));
        
        flights.push_back(flight);
    }
    
    file.close();
    return flights;
}



void FileManager::saveFlights(const vector<Flight>& flights) {
    std::cout << "saveFlights called with " << flights.size() << " flights" << std::endl;
    std::cout << "Trying to open: " << FLIGHTS_FILE << std::endl;
    
    ofstream file(FLIGHTS_FILE);
    
    if (!file.is_open()) {
        cerr << "FAILED TO OPEN FILE: " << FLIGHTS_FILE << endl;
        return;
    }
    
    std::cout << "File opened successfully!" << std::endl;
    
    for (const auto& flight : flights) {
        file << flight.getFlightNo() << "," << flight.getAirline() << "," << flight.getSource() << "," << flight.getDestination() << "," << flight.getdepartTime() << ","
                << flight.getArrivalTime() << "," << flight.getdepartDate() << "," << fsToString(flight.getStatus()) << "," << flight.getTotalSeats() << ","
                    << flight.getAvailableSeats() << ","<< flight.getBaseFare() << "\n";
    }
    
    file.close();
    std::cout << "File closed, save complete!" << std::endl;
}

vector<shared_ptr<Passenger>> FileManager::loadPassengers(vector<Flight>& flights) {
    vector<shared_ptr<Passenger>> passengers;
    ifstream file(PASSENGERS_FILE);
    
    if (!file.is_open()) {
        // cerr << "Unable\n";
        return passengers;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string id, name, email, phone, passportNo, nationality;
        int age, numTickets;
        double walletBalance;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, phone, ',');
        ss >> age;
        ss.ignore();
        getline(ss, passportNo, ',');
        getline(ss, nationality, ',');
        ss >> walletBalance;
        ss.ignore();
        ss >> numTickets;
        ss.ignore();
        
        auto passenger = make_shared<Passenger>(id, name, email, phone, age, passportNo, nationality);
        if (walletBalance > 0) {
            passenger->addToWallet(walletBalance);
        }
        
        // load tickets
        for (int i = 0; i < numTickets; i++) {
            string ticketId, flightNo, bookingDate, statusStr, seatClassStr, paymentStr;
            int seatNumber, hasMeal, hasInsurance;
            double price;
            
            getline(ss, ticketId, ',');
            getline(ss, flightNo, ',');
            ss >> seatNumber;
            ss.ignore();
            getline(ss, seatClassStr, ',');
            ss >> price;
            ss.ignore();
            getline(ss, statusStr, ',');
            getline(ss, paymentStr, ',');
            ss >> hasMeal;
            ss.ignore();
            ss >> hasInsurance;
            ss.ignore();
            getline(ss, bookingDate, ',');
            
            // find the flight
            Flight* flightPtr = nullptr;
            for (auto& flight : flights) {
                if (flight.getFlightNo() == flightNo) {
                    flightPtr = &flight;
                    break;
                }
            }

            
            flightPtr->bookSeat(seatNumber);
            auto ticket = make_shared<Ticket>(
                make_shared<Flight>(*flightPtr),
                passenger,
                seatNumber,
                stringTosc(seatClassStr),
                price,
                stringTopm(paymentStr)
            );
            
            ticket->setHasMeal(hasMeal == 1);
            ticket->setHasInsurance(hasInsurance == 1);
            
            passenger->addTicket(ticket);
        }
        
        passengers.push_back(passenger);
    }
    
    file.close();
    return passengers;
}


void FileManager::savePassengers(const vector<shared_ptr<Passenger>>& passengers) {
    ofstream file(PASSENGERS_FILE);
    
    if (!file.is_open()) {
        cerr << "Unable to open passengers file for writing.\n";
        return;
    }
    
    for (const auto& passenger : passengers) {
        const auto& tickets = passenger->getBookedTickets();
        
        file << passenger->getId() << "," << passenger->getName() << "," << passenger->getEmail() << "," << passenger->getPhone() << "," << passenger->getAge() << "," << passenger->getPassportNo() << ","
                << passenger->getNationality() << ","<< passenger->getWalletBalance() << ","<< tickets.size();
        
        // Save all tickets for this passenger
        for (const auto& ticket : tickets) {
            file << "," << ticket->getTicketId() << "," << ticket->getFlight()->getFlightNo() << "," << ticket->getSeatNumber() << "," << scToString(ticket->getSeatClass()) << "," << ticket->getPrice()
                 << "," << pmToString(ticket->getPaymentMethod()) << "," << (ticket->getHasMeal() ? 1 : 0)<< "," << (ticket->getHasInsurance() ? 1 : 0)<< "," << ticket->getBookingDate();
        }
        
        file << "\n";
    }
    

    file.close();
}

vector<shared_ptr<Admin>> FileManager::loadAdmins() {
    vector<shared_ptr<Admin>> admins;
    ifstream file(ADMINS_FILE);
    
    if (!file.is_open()) {
        // cerr << "admins file prob\n";
        return admins;
    }
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, email, phone, username, password;
        int age;
        
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, phone, ',');
        ss >> age;
        ss.ignore();
        getline(ss, username, ',');
        getline(ss, password);
        

        auto admin = make_shared<Admin>(id, name, email, phone, age, username, password);
        admins.push_back(admin);
    }
    file.close();
    return admins;
}

void FileManager::saveAdmins(const vector<shared_ptr<Admin>>& admins) {
    ofstream file(ADMINS_FILE);
    
    if (!file.is_open()) {
        return;
    }
    for (const auto& admin : admins) {
        file << admin->getId() << "," << admin->getName() << ","<< admin->getEmail() << "," << admin->getPhone() << "," << admin->getAge() << "," << admin->getUsername() << "," << admin->getPassword() << "\n";
    }
    
    file.close();
}