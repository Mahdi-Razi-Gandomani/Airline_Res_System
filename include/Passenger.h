#ifndef PASSENGER_H
#define PASSENGER_H

#include "Person.h"
#include <vector>
#include <memory>
using namespace std;
class Ticket;

class Passenger : public Person {
private:
    string passportNo;
    string nationality;
    vector<shared_ptr<Ticket>> bookedTickets;
    double walletBalance;

public:
    Passenger();
    Passenger(const string& id, const string& name, const string& email, const string& phone, int age,
              const string& passportNo, const string& nationality);


    string getPassportNo() const { return passportNo; }
    string getNationality() const { return nationality; }
    double getWalletBalance() const { return walletBalance;}
    const vector<shared_ptr<Ticket>>& getBookedTickets() const { 
        return bookedTickets; 
    }


    void setPassportNo(const string& passport) { passportNo = passport; }
    void setNationality(const string& nat) { nationality = nat;}
    void addToWallet(double amount);
    bool dedWallet(double amount);

    void addTicket(shared_ptr<Ticket> ticket);
    bool removeTicket(const string& ticketId);
    void viewTickets() const;

    // Override
    void displayInfo() const override;
    string getRole() const override { return "Passenger";}

};
#endif