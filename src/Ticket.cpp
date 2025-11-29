#include "Ticket.h"
#include "Flight.h"
#include "Passenger.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <random>
using namespace std;

int Ticket::ticketCounter = 1000;



Ticket::Ticket() : ticketId(""), seatNumber(0), seatClass(SeatClass::ECONOMY), price(0.0),  bookingDate(""), 
paymentMethod(PaymentMethod::CREDIT_CARD),hasMeal(false), hasInsurance(false) {}

Ticket::Ticket(shared_ptr<Flight> flt, shared_ptr<Passenger> pass, int seat, SeatClass cls, double price, PaymentMethod payment): 
    flight(flt), passenger(pass), seatNumber(seat), seatClass(cls), price(price),paymentMethod(payment),
      hasMeal(false), hasInsurance(false) {
    
    ticketId = "TCK" + to_string(++ticketCounter);
    
    // Get current date
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    bookingDate = buffer;
}



void Ticket::printTicket() const {
    cout << "\n========== TICKET DETAILS ==========\n";
    cout << "Ticket ID: " << ticketId << "\n";
    cout << "Passenger: " << passenger->getName() << "\n";
    cout << "Passport: " << passenger->getPassportNo() << "\n";
    cout << "Flight: " << flight->getFlightNo() << "\n";
    cout  << flight->getSource() << " → " << flight->getDestination() << "\n";
    cout << "Date: " << flight->getdepartDate() << "\n";
    cout << "Departure: " << flight->getdepartTime() << "\n";
    cout << "Arrival: " << flight->getArrivalTime() << "\n";

    cout << "Seat: "  << seatNumber << "\n";
    cout << "Class: "  
              << (seatClass == SeatClass::ECONOMY ? "Economy" :
                  seatClass == SeatClass::BUSINESS ? "Business" : "First Class") << "\n";
    cout << "Price: $" << fixed << setprecision(2)  << price << "\n";
    cout << "Meal: "  << (hasMeal ? "Yes" : "No") << "\n";
    cout << "Insurance: "  << (hasInsurance ? "Yes" : "No") << "\n";
    cout << "Booked: " << bookingDate << "\n";
    cout << "=====================================\n";
}

// void Ticket::printBoardingPass() const {
//     cout << "\n";
//     cout << "┌────────────────────────────────────────┐\n";
//     cout << "│           BOARDING PASS                │\n";
//     cout << "├────────────────────────────────────────┤\n";
//     cout << "│ " << left << setw(38) << passenger->getName() << " │\n";
//     cout << "│ Flight: " << setw(30) << flight->getFlightNo() << " │\n";
//     cout << "│ " << setw(15) << flight->getSource() << " → " << setw(19) << flight->getDestination() << " │\n";
//     cout << "│ Seat: " << setw(33) << seatNumber << " │\n";
//     cout << "│ Boarding: " << setw(29) << flight->getdepartTime() << " │\n";
//     cout << "└────────────────────────────────────────┘\n";
// }

// bool Ticket::checkIn() {
//     if (status == TicketStatus::CONFIRMED) {
//         status = TicketStatus::CHECKED_IN;
//         cout << "Check-in successful!\n";
//         printBoardingPass();
//         return true;
//     }
//     cout << "Cannot check in. Ticket status: " << getStatusString() << "\n";
//     return false;
// }


