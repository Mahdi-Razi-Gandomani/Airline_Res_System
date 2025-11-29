#include "ReservationSystem.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
using namespace std;


ReservationSystem* ReservationSystem::instance = nullptr;

ReservationSystem::ReservationSystem(): currentPassenger(nullptr), currentAdmin(nullptr) {}

ReservationSystem* ReservationSystem::getInstance() {
    if (instance == nullptr) {
        instance = new ReservationSystem();
    }
    return instance;
}

ReservationSystem::~ReservationSystem() {
    saveAllData();
}

void ReservationSystem::loadAllData() {
    fileManager.initDataFiles();
    flights = fileManager.loadFlights();
    passengers = fileManager.loadPassengers(flights);
    admins = fileManager.loadAdmins();
}

void ReservationSystem::saveAllData() {
    fileManager.saveFlights(flights);
    fileManager.savePassengers(passengers);
    fileManager.saveAdmins(admins);
}

void ReservationSystem::initSampleData() {
    
    /////////
    flights.push_back(Flight("AA101", "American Airlines", "New York", "Los Angeles", "08:00", "11:30", "2025-11-15", 180, 250.0));
    flights.push_back(Flight("UA202", "United Airlines", "New York", "Los Angeles", "14:30", "19:00", "2025-11-15", 200, 300.0));
    flights.push_back(Flight("DL303", "Delta Airlines", "Boston", "San Francisco", "06:45", "10:15", "2025-11-16", 220, 350.0));
    flights.push_back(Flight("SW404", "American Airlines", "Dallas", "Seattle", "12:00", "14:30", "2025-11-16", 150, 200.0));
    flights.push_back(Flight("BA505", "British Airways", "London", "New York", "18:00", "21:00", "2025-11-17", 300, 800.0));
    ////////


    auto admin = make_shared<Admin>("A001", "Admin User", "admin@airline.com", "+1234567890", 35, "admin", "admin123");
    admins.push_back(admin);
}

shared_ptr<Passenger> ReservationSystem::regPass( const string& name, const string& email,const string& phone, int age, const string& passport,
    const string& nationality) {
    
    // Check if passenger already exists
    for (const auto& p : passengers) {
        if (p->getEmail() == email || p->getPassportNo() == passport) {
            cout << "Passenger with this email or pass already exists.\n";
            return nullptr;
        }
    }

    string id = "P" + to_string(passengers.size() + 1001);
    auto passenger = make_shared<Passenger>(id, name, email, phone, age, passport, nationality);
    passengers.push_back(passenger);
    // currentPassenger = passenger;
    cout << "Signup successful! Your Passenger ID: " << id << "\n";
    return passenger;
}

shared_ptr<Passenger> ReservationSystem::logPass ( const string& email, const string& passport) {
    for (const auto& p : passengers) {
        if (p->getEmail() == email && p->getPassportNo() == passport) {
            currentPassenger = p;
            // cout << "Welcome back, " << p->getName() << "!\n";
            return p;
        }
    }
    
    cout << "Invalid info. Please try again.\n";
    return nullptr;
}

void ReservationSystem::logoutPass() {
    if (currentPassenger) {
        cout << "Goodbye, " << currentPassenger->getName() << "!\n";
        currentPassenger = nullptr;
    }
}

shared_ptr<Admin> ReservationSystem::loginAdmin(
    const string& username, const string& password) {
    for (const auto& a : admins) {
        if (a->auth(username, password)) {
            currentAdmin = a;
            cout << "Welcome, " << a->getName() << "!\n";
            return a;
        }
    }
    
    cout << "Invalid admin info.\n";
    return nullptr;
}

void ReservationSystem::logoutAdmin() {
    if (currentAdmin) {
        cout << "Admin logged out.\n";
        currentAdmin = nullptr;
    }
}

vector<Flight> ReservationSystem::searchFlights(const string& source, const string& destination,const string& date) {
    vector<Flight> results;
    for (const auto& flight : flights) {
        bool matchSource = source.empty() || flight.getSource() == source;
        bool matchDest = destination.empty() || flight.getDestination() == destination;
        bool matchDate = date.empty() || flight.getdepartDate() == date;
        
        if (matchSource && matchDest && matchDate && 
            flight.getAvailableSeats() > 0) {
            results.push_back(flight);
        }
    }
    return results;
}

Flight* ReservationSystem::getFlightbyNo(const string& flightNo) {
    for (auto& flight : flights) {
        if (flight.getFlightNo() == flightNo) {
            return &flight;
        }
    }
    return nullptr;
}

bool ReservationSystem::addFlight(const Flight& flight) {
    if (getFlightbyNo(flight.getFlightNo()) != nullptr) {
        cout << "Flight with this number already exists.\n";
        return false;
    }
    flights.push_back(flight);
    cout << "Flight " << flight.getFlightNo() << " added succesfully.\n";
    return true;
}



shared_ptr<Ticket> ReservationSystem::bookTicket(const string& flightNo, int seatNumber,SeatClass seatClass, PaymentMethod payment,bool hasMeal, bool hasInsurance) {
    
    if (!currentPassenger) {
        cout << "Please login first.\n";
        return nullptr;
    }

    Flight* flight = getFlightbyNo(flightNo);
    if (!flight) {
        cout << "Flight not found.\n";
        return nullptr;
    }

    Seat* seat = flight->getSeat(seatNumber);
    if (!seat || !seat->isAvailable) {
        cout << "Seat not available.\n";
        return nullptr;
    }

    double totalPrice = seat->price;
    if (hasMeal) totalPrice += 25.0;
    if (hasInsurance) totalPrice += 50.0;

    // Handle payment
    bool paymentSuccess = false;
    if (payment == PaymentMethod::WALLET) {
        paymentSuccess = currentPassenger->dedWallet(totalPrice);
        if (!paymentSuccess) {
            cout << "wallet balance not enough.\n";
            return nullptr;
        }
    } else {
        paymentSuccess = true; // Simulated payment
        cout << "Payment of $" << fixed << setprecision(2) << totalPrice << "\n";
    }

    if (!flight->bookSeat(seatNumber)) {
        cout << "Failed to book.\n";
        return nullptr;
    }


    auto ticket = make_shared<Ticket>(make_shared<Flight>(*flight), currentPassenger, seatNumber, seatClass, totalPrice, payment);
    
    ticket->setHasMeal(hasMeal);
    ticket->setHasInsurance(hasInsurance);
    currentPassenger->addTicket(ticket);
    cout << "\nBooking Successful!\n";
    ticket->printTicket();
    return ticket;
}

bool ReservationSystem::cancelTicket(const string& ticketId) {
    if (!currentPassenger) {
        cout << "Please login first.\n";
        return false;
    }

    // find the ticket
    const auto& tickets = currentPassenger->getBookedTickets();
    shared_ptr<Ticket> ticketToCancel = nullptr;
    for (const auto& t : tickets) {
        if (t->getTicketId() == ticketId) {
            ticketToCancel = t;
            break;
        }
    }

    if (!ticketToCancel) {
        cout << "Ticket not found.\n";
        return false;
    }

    // cancel the seat on flight
    Flight* flight = getFlightbyNo(ticketToCancel->getFlight()->getFlightNo());
    if (!flight->cancelSeat(ticketToCancel->getSeatNumber())) {
        cout << "Cancellation failed.\n";
        return false;
    }

    double refund;
    if (flight->getStatus() == FlightStatus::CANCELLED) {
        refund = ticketToCancel->getPrice();
    }
    else {
        refund = ticketToCancel->getPrice() * 0.8;
    }
    currentPassenger->addToWallet(refund);

    //Remove the ticket from the passenger’s booked list
    if (currentPassenger->removeTicket(ticketId)) {
        cout << "Ticket cancelled successfully. Refund of $" << fixed << setprecision(2) << refund << " to wallet.\n";
        return true;
    }

    cout << "Failed to remove ticket.\n";
    return false;
}



void ReservationSystem::displayAllFlights() {
    if (flights.empty()) {
        cout << "No flights available.\n";
        return;
    }

    cout << "\n---------- Available Flights ----------\n";

    for (const auto& flight : flights) {
        if (flight.getAvailableSeats() > 0) {
            flight.displayInfo();
        }
    }
    cout << "\n";
}

