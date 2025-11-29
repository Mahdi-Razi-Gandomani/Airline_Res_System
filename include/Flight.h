#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <vector>
#include <map>
#include <ctime>

using namespace std;

enum class FlightStatus {
    SCHEDULED,
    DEPARTED,
    ARRIVED,
    DELAYED,
    CANCELLED
};

enum class SeatClass {
    ECONOMY,
    BUSINESS,
    FIRST_CLASS
};


struct Seat {
    int seatNumber;
    SeatClass seatClass;
    bool isAvailable;
    double price;

    Seat(int num, SeatClass cls, double pr): 
    seatNumber(num), seatClass(cls), isAvailable(true), price(pr) {}
};



class Flight {
private:
    string flightNo;
    string airline;
    string source;
    string destination;
    string departTime;
    string arrivalTime;
    string departDate;
    FlightStatus status;
    vector<Seat> seats;
    int totalSeats;
    int availableSeats;
    double baseFare;


public:
    Flight();
    Flight(const string& flightNo, const string& airline,const string& src, const string& dest,const string& depTime, const string& arrTime,
    const string& depDate, int totalSeats, double base);


    string getFlightNo() const { return flightNo; }
    string getAirline() const { return airline; }
    string getSource() const { return source; }
    string getDestination() const { return destination; }
    string getdepartTime() const { return departTime;}
    string getArrivalTime() const { return arrivalTime; }
    string getdepartDate() const { return departDate; }
    FlightStatus getStatus() const { return status; }
    int getTotalSeats() const { return totalSeats;}
    int getAvailableSeats() const { return availableSeats; }
    double getBaseFare() const {return baseFare; }


    void setStatus(FlightStatus st) { status = st; }
    void setdepartTime(const string& time) { departTime = time; }
    void setArrivalTime(const string& time) {arrivalTime = time; }
    void setBaseFare(double bf) {baseFare = bf;}


    bool bookSeat(int seatNumber);
    bool cancelSeat(int seatNumber);
    vector<Seat> getAvailableSeats(SeatClass cls) const;
    Seat* getSeat(int seatNumber);

    // Display
    void displayInfo() const;
    string getStatusString() const;

};

#endif