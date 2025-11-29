#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <memory>
#include <ctime>
#include "Flight.h"

using namespace std;
class Flight;
class Passenger;


enum class PaymentMethod {
    CREDIT_CARD,
    WALLET
};

class Ticket {
private:
    string ticketId;
    shared_ptr<Flight> flight;
    shared_ptr<Passenger> passenger;
    int seatNumber;
    SeatClass seatClass;
    double price;
    string bookingDate;
    string pnr;
    PaymentMethod paymentMethod;
    bool hasMeal;
    bool hasInsurance;

    static int ticketCounter;
    string generatePNR();

public:
    Ticket();
    Ticket(shared_ptr<Flight> flt, shared_ptr<Passenger> pass,int seat, SeatClass cls, double price, PaymentMethod payment);


    string getTicketId() const { return ticketId;}
    shared_ptr<Flight> getFlight() const { return flight;}
    shared_ptr<Passenger> getPassenger() const { return passenger;}
    int getSeatNumber() const { return seatNumber; }
    SeatClass getSeatClass() const { return seatClass; }
    double getPrice() const { return price; }
    string getBookingDate() const { return bookingDate; }
    bool getHasMeal() const { return hasMeal; }
    bool getHasInsurance() const { return hasInsurance;}
    PaymentMethod getPaymentMethod() const {return paymentMethod;}


    void setHasMeal(bool meal) { hasMeal = meal; }
    void setHasInsurance(bool ins) { hasInsurance = ins;}


    void printTicket() const;
    // void printBoardingPass() const;
    // bool checkIn();

};


#endif