#ifndef RESERVATIONSYSTEM_H
#define RESERVATIONSYSTEM_H

#include <vector>
#include <memory>
#include <map>
#include <string>
#include "Flight.h"
#include "Passenger.h"
#include "Admin.h"
#include "Ticket.h"
#include "FileManager.h"

using namespace std;

class ReservationSystem {
private:
    vector<Flight> flights;
    vector<shared_ptr<Passenger>> passengers;
    vector<shared_ptr<Admin>> admins;
    vector<shared_ptr<Ticket>> tickets;
    FileManager fileManager;
    
    shared_ptr<Passenger> currentPassenger;
    shared_ptr<Admin> currentAdmin;

    // Singleton instance
    static ReservationSystem* instance;
    ReservationSystem();

public:
    // Singleton access
    static ReservationSystem* getInstance();
    ~ReservationSystem();


    void loadAllData();
    void saveAllData();
    void initSampleData();

    //passenger
    shared_ptr<Passenger> regPass(const string& name, const string& email,const string& phone, int age, const string& passport, const string& nationality);
    shared_ptr<Passenger> logPass(const string& email, const string& passport);
    void logoutPass();

    // Admin
    shared_ptr<Admin> loginAdmin(const string& username, const string& password);
    void logoutAdmin();

    // flight
    vector<Flight> searchFlights(const string& source, const string& destination,const string& date);
    vector<Flight> getAllFlights() const { return flights; }
    Flight* getFlightbyNo(const string& flightNo);
    bool addFlight(const Flight& flight);
    bool updateFlight(const string& flightNo, const Flight& updatedFlight);


    
    shared_ptr<Ticket> bookTicket( const string& flightNo, int seatNumber,SeatClass seatClass, PaymentMethod payment,bool hasMeal, bool hasInsurance);
    bool cancelTicket(const string& ticketId);
    shared_ptr<Ticket> getTicketById(const string& ticketId);


    shared_ptr<Passenger> getThisPass() const { 
        return currentPassenger; 
    }
    // shared_ptr<Admin> getThisAdmin() const { 
    //     return currentAdmin; 
    // }


    void displayAllFlights();
};

#endif