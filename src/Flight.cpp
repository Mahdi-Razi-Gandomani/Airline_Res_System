#include "Flight.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

Flight::Flight() 
    : flightNo(""), airline(""), source(""), destination(""), departTime(""), arrivalTime(""), departDate(""),
      status(FlightStatus::SCHEDULED), totalSeats(0), availableSeats(0), baseFare(0) {}

Flight::Flight(const string& flightNo, const string& airline, const string& src, const string& dest, const string& depTime, const string& arrTime,
               const string& depDate, int totalSeats, double bf) :
     flightNo(flightNo), airline(airline), source(src), destination(dest), departTime(depTime), arrivalTime(arrTime), departDate(depDate), status(FlightStatus::SCHEDULED),
      totalSeats(totalSeats), availableSeats(totalSeats), baseFare(bf) {

    seats.clear();
    int economySeats = totalSeats * 0.7;
    int businessSeats = totalSeats * 0.2;
    // int firstClassSeats = totalSeats * 0.1;

    for (int i = 1; i <= economySeats; ++i) {
        seats.emplace_back(i, SeatClass::ECONOMY, baseFare);
    }
 
    for (int i = economySeats + 1; i <= economySeats + businessSeats; ++i) {
        seats.emplace_back(i, SeatClass::BUSINESS, baseFare * 2);
    }

    for (int i = economySeats + businessSeats + 1; i <= totalSeats; ++i) {
        seats.emplace_back(i, SeatClass::FIRST_CLASS, baseFare * 3);
    }
}

bool Flight::bookSeat(int seatNumber) {
    for (auto& seat : seats) {
        if (seat.seatNumber == seatNumber && seat.isAvailable) {
            seat.isAvailable = false;
            availableSeats--;
            return true;
        }
    }
    return false;
}

bool Flight::cancelSeat(int seatNumber) {
    for (auto& seat : seats) {
        if (seat.seatNumber == seatNumber && !seat.isAvailable) {
            seat.isAvailable = true;
            availableSeats++;
            return true;
        }
    }
    return false;
}

vector<Seat> Flight::getAvailableSeats(SeatClass cls) const {
    vector<Seat> available;
    for (const auto& seat : seats) {
        if (seat.isAvailable && seat.seatClass == cls) {
            available.push_back(seat);
        }
    }
    return available;
}

Seat* Flight::getSeat(int seatNumber) {
    for (auto& seat : seats) {
        if (seat.seatNumber == seatNumber) {
            return &seat;
        }
    }
    return nullptr;
}


void Flight::displayInfo() const {
    cout << "\n========== FLIGHT DETAILS ==========\n";
    cout << "Flight Number: " << flightNo << "\n";
    cout << "Airline: " << airline << "\n";
    cout << "Route: " << source << " → " << destination << "\n";
    cout << "Date: " << departDate << "\n";
    cout << "Departure: " << departTime << "\n";
    cout << "Arrival: " << arrivalTime << "\n";
    cout << "Status: " << getStatusString() << "\n";
    cout << "Available Seats: " << availableSeats << "/" << totalSeats << "\n";
    cout << "Base Fare: $" << fixed << setprecision(2) << baseFare << "\n";
    cout << "=====================================\n";
}

string Flight::getStatusString() const {
    switch (status) {
        case FlightStatus::SCHEDULED: return "Scheduled";
        case FlightStatus::DEPARTED: return "Departed";
        case FlightStatus::ARRIVED: return "Arrived";
        case FlightStatus::DELAYED: return "Delayed";
        case FlightStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}