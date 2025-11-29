#include "Passenger.h"
#include "Ticket.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;



Passenger::Passenger() : Person(), passportNo(""), nationality(""), walletBalance(0.0) {}

Passenger::Passenger(const string& id, const string& name,const string& email, const string& phone, int age,const string& passportNo
    , const string& nationality) : Person(id, name, email, phone, age), passportNo(passportNo), nationality(nationality), walletBalance(0.0) {}

void Passenger::addToWallet(double amount) {
    if (amount > 0) {
        walletBalance += amount;
        cout << "Added $" << fixed << setprecision(2) << amount << " to wallet. New balance: $" << walletBalance << "\n";
    }
}

bool Passenger::dedWallet(double amount) {
    if (amount > 0 && walletBalance >= amount) {
        walletBalance -= amount;
        return true;
    }
    return false;
}

void Passenger::addTicket(shared_ptr<Ticket> ticket) {
    bookedTickets.push_back(ticket);
}

bool Passenger::removeTicket(const string& ticketId) {
    for (auto it = bookedTickets.begin(); it != bookedTickets.end(); ++it) {
        if ((*it)->getTicketId() == ticketId) {
            bookedTickets.erase(it);
            return true;
        }
    }
    return false;
}

void Passenger::viewTickets() const {
    if (bookedTickets.empty()) {
        cout << "No tickets booked.\n";
        return;
    }

    for (const auto& ticket : bookedTickets) {
        ticket->printTicket();
        cout << "------------------------------------\n";
    }
}

void Passenger::displayInfo() const {
    Person::displayInfo();
    cout << "Passport No: " << passportNo << "\n" << "Nationality: " << nationality << "\n" << "Wallet Balance: $" << fixed << setprecision(2) << walletBalance << "\n"
            << "Total Bookings: " << bookedTickets.size() << "\n";
}
