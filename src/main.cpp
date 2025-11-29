#include <iostream>
#include <limits>
#include <string>
#include "ReservationSystem.h"

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\nPress Enter to continue";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void displayMainMenu() {
    cout << "\n";
    cout << "╔═════════════════════════════════╗\n";
    cout << "║   AIRLINE RESERVATION SYSTEM    ║\n";
    cout << "╠═════════════════════════════════╣\n";
    cout << "║  1. Passenger Login             ║\n";
    cout << "║  2. Passenger Registration      ║\n";
    cout << "║  3. Admin Login                 ║\n";
    cout << "║  4. View All Flights            ║\n";
    cout << "║  5. Search Flights              ║\n";
    cout << "║  0. Exit                        ║\n";
    cout << "╚═════════════════════════════════╝\n";
    cout << "Enter choice: ";
}

void displayPassengerMenu() {
    cout << "\n";
    cout << "╔════════════════════════════════╗\n";
    cout << "║         PASSENGER MENU         ║\n";
    cout << "╠════════════════════════════════╣\n";
    cout << "║  1. View All Flights           ║\n";
    cout << "║  2. Search Flights             ║\n";
    cout << "║  3. Book a Flight              ║\n";
    cout << "║  4. View My Tickets            ║\n";
    cout << "║  5. Cancel Booking             ║\n";
    cout << "║  6. Add Money to Wallet        ║\n";
    cout << "║  7. View Profile               ║\n";
    cout << "║  0. Logout                     ║\n";
    cout << "╚════════════════════════════════╝\n";
    cout << "Enter choice: ";
}

void displayAdminMenu() {
    cout << "\n";
    cout << "╔═══════════════════════════════╗\n";
    cout << "║           ADMIN MENU          ║\n";
    cout << "╠═══════════════════════════════╣\n";
    cout << "║  1. View All Flights          ║\n";
    cout << "║  2. Add New Flight            ║\n";
    cout << "║  3. Update Flight Status      ║\n";
    cout << "║  0. Logout                    ║\n";
    cout << "╚═══════════════════════════════╝\n";
    cout << "Enter choice: ";
}

void passengersignup(ReservationSystem* system) {
    string name, email, phone, passport, nationality;
    int age;

    cout << "\nPassenger Signup\n";
    cin.ignore();
    cout << "Full Name: ";
    getline(cin, name);
    cout << "Email: ";
    getline(cin, email);
    cout << "Phone: ";
    getline(cin, phone);
    cout << "Age: ";
    cin >> age;
    cin.ignore();
    cout << "Passport Number: ";
    getline(cin, passport);
    cout << "Nationality: ";
    getline(cin, nationality);

    system->regPass(name, email, phone, age, passport, nationality);
    pause();
}

void passengerLogin(ReservationSystem* system) {
    string email, passport;
    
    cout << "\nPassenger Login\n";
    cin.ignore();
    cout << "Email: ";
    getline(cin, email);
    cout << "Passport Number: ";
    getline(cin, passport);

    auto passenger = system->logPass(email, passport);
    if (!passenger) {
        pause();
        return;
    }

    int choice;
    do {
        clearScreen();
        displayPassengerMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                system->displayAllFlights();
                pause();
                break;

            case 2: {
                string source, dest, date;
                cin.ignore();
                cout << "Source (or Enter to skip): ";
                getline(cin, source);
                cout << "Destination (or Enter to skip): ";
                getline(cin, dest);
                cout << "Date (YYYY-MM-DD, or Enter to skip): ";
                getline(cin, date);

                auto results = system->searchFlights(source, dest, date);
                if (results.empty()) {
                    cout << "No flights found.\n";
                } else {
                    cout << "\n--- Search Results ---\n";
                    for (const auto& flight : results) {
                        flight.displayInfo();
                    }
                }
                pause();
                break;
            }

            case 3: {
                string flightNo;
                int seatNo, classChoice, paymentChoice;
                char mealChoice, insChoice;

                system->displayAllFlights();
                cin.ignore();
                cout << "\nFlight Number: ";
                getline(cin, flightNo);

                auto flight = system->getFlightbyNo(flightNo);
                if (!flight) {
                    cout << "Invalid\n";
                    pause();
                    break;
                }

                cout << "\nSelect Class:\n";
                cout << "1. Econ\n2. Business\n3. First Class\n";
                cout << "Choice: ";
                cin >> classChoice;

                SeatClass seatClass = static_cast<SeatClass>(classChoice - 1);
                auto availSeats = flight->getAvailableSeats(seatClass);
                if (availSeats.empty()) {
                    cout << "No seats available in this class.\n";
                    pause();
                    break;
                }

                cout << "\nAvailable seats:";
                for (const auto& seat : availSeats) {
                    cout << seat.seatNumber << " ($" << seat.price << ") ";
                }
                cout << "\nEnter seat number: ";
                cin >> seatNo;

                cout << "\nSelect Payment method:\n";
                cout << "1. Credit Card\n2. Wallet\n";
                cout << "Choice: ";
                cin >> paymentChoice;

                cout << "Add meal? y or n? ";
                cin >> mealChoice;
                cout << "Add insurance? y or n? ";
                cin >> insChoice;

                auto ticket = system->bookTicket(flightNo, seatNo, seatClass, static_cast<PaymentMethod>(paymentChoice - 1), mealChoice == 'y' || mealChoice == 'Y', insChoice == 'y' || insChoice == 'Y');

                pause();
                break;
            }

            case 4: // View tickets
                passenger->viewTickets();
                pause();
                break;

            case 5: { // Cancel booking
                string ticketId;
                passenger->viewTickets();
                cin.ignore();
                cout << "\nEnter Ticket ID to cancel: ";
                getline(cin, ticketId);
                system->cancelTicket(ticketId);
                pause();
                break;
            }


            case 6: { // Add money
                double amount;
                cout << "Enter amount to add: $";
                cin >> amount;
                passenger->addToWallet(amount);
                pause();
                break;
            }

            case 7: // View profile
                passenger->displayInfo();
                pause();
                break;

            case 0:
                system->logoutPass();
                break;

            default:
                cout << "Invalid choice.\n";
                pause();
        }
    } while (choice != 0);
}

void adminLogin(ReservationSystem* system) {
    string username, password;
    
    cout << "\nAdmin Login\n";
    cin.ignore();
    cout << "Username: ";
    getline(cin, username);
    cout << "Password: ";
    getline(cin, password);

    auto admin = system->loginAdmin(username, password);
    if (!admin) {
        pause();
        return;
    }

    int choice;
    do {
        clearScreen();
        displayAdminMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                system->displayAllFlights();
                pause();
                break;

            case 2: {
                string flightNo, airline, src, dest, depTime, arrTime, date;
                int seats;
                double fare;

                cin.ignore();
                cout << "\n--- Add Flight ---\n";
                cout << "Flight Num: "; getline(cin, flightNo);
                cout << "Airline: "; getline(cin, airline);
                cout << "Source: "; getline(cin, src);
                cout << "Destination: "; getline(cin, dest);
                cout << "Depart time (HH:MM): "; getline(cin, depTime);
                cout << "Arrival time (HH:MM): "; getline(cin, arrTime);
                cout << "Date (YYYY-MM-DD): "; getline(cin, date);
                cout << "Total seats: "; cin >> seats;
                cout << "Base price: $"; cin >> fare;

                Flight newFlight(flightNo, airline, src, dest, depTime, arrTime, date, seats, fare);
                system->addFlight(newFlight);
                pause();
                break;
            }


            case 3: { // Update status
                string flightNo;
                int statusChoice;
                cin.ignore();
                cout << "Enter Flight Number: ";
                getline(cin, flightNo);
                
                auto flight = system->getFlightbyNo(flightNo);
                if (flight) {
                    cout << "\nSelect Status:\n";
                    cout << "0. Scheduled\n1.. Departed\n" << "2. Arrived \n3. Delayed\n4. Cancelled\n";
                    cout << "Choice: ";
                    cin >> statusChoice;
                    flight->setStatus(static_cast<FlightStatus>(statusChoice));
                    cout << "Status updated.\n";
                } else {
                    cout << "Flight not found.\n";
                }
                pause();
                break;
            }


            case 0:
                system->logoutAdmin();
                break;

            default:
                cout << "Invalid choice.\n";
                pause();
        }
    } while (choice != 0);
}

int main() {
    ReservationSystem* system = ReservationSystem::getInstance();
    
    system->loadAllData();
    
    // Initialize sample data if no flights
    if (system->getAllFlights().empty()) {
        system->sampleData();
    }

    int choice;
    do {
        clearScreen();
        displayMainMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                passengerLogin(system);
                break;
            case 2:
                passengersignup(system);
                break;
            case 3:
                adminLogin(system);
                break;
            case 4:
                system->displayAllFlights();
                pause();
                break;
            case 5: {
                string source, dest, date;
                cin.ignore();
                cout << "Source: ";
                getline(cin, source);
                cout << "Destination: ";
                getline(cin, dest);
                cout << "Date (YYYY-MM-DD): ";
                getline(cin, date);
                
                auto results = system->searchFlights(source, dest, date);
                if (results.empty()) {
                    cout << "No flights found.\n";
                } else {
                    for (const auto& flight : results) {
                        flight.displayInfo();
                    }
                }
                pause();
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice, try again.\n";
                pause();
        }
    } while (choice != 0);

    system->saveAllData();
    delete system;
    return 0;
}