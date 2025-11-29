#ifndef FILEMANAGER_H
#define FILEMANAGER_H


#include <string>
#include <vector>
#include <memory>
#include "Flight.h"
#include "Passenger.h"
#include "Admin.h"
#include "Ticket.h"

using namespace std;

class FileManager {
private:
    const string FLIGHTS_FILE = "data/flights.txt";
    const string PASSENGERS_FILE = "data/passengers.txt";
    const string ADMINS_FILE = "data/admins.txt";

public:
    FileManager();
    void initDataFiles();
    // Flight
    vector<Flight> loadFlights();
    void saveFlights(const vector<Flight>& flights);
    
    // Passenger
    vector<shared_ptr<Passenger>> loadPassengers(vector<Flight>& flights);
    void savePassengers(const vector<shared_ptr<Passenger>>& passengers);
    
    // Admin
    vector<shared_ptr<Admin>> loadAdmins();
    void saveAdmins(const vector<shared_ptr<Admin>>& admins);
    
private:
    // Helpers
    bool fileExists(const string& filename);
    string scToString(SeatClass cls);
    SeatClass stringTosc(const string& str);
    string fsToString(FlightStatus status);
    FlightStatus stringTofs(const string& str);
    string pmToString(PaymentMethod pm);
    PaymentMethod stringTopm(const string& str);
};
#endif 