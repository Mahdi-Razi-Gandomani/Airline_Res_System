#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include "ReservationSystem.h"
#include "ChatBot.h"
#include "ChatbotWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onlogin();
    void onregister();
    void onAdminLogin();
    void onLogout();
    

    void onSearchFlights();
    void onBookFlight();
    // void onViewMyTickets();////////////
    void onCancelBooking();
    void onAddToWallet();
    

    void onAddFlight();
    // void status();/////
    void onAdminSearch();
    void onUpdateFlight();
    
 
    void onFlightSelected(int row, int column);
    void onTicketSelected(int row, int column);
    void updateBookingPrice();

private:
    enum ScreenIndex {
        WELCOME = 0,
        PASSENGER_LOGIN,
        PASSENGER_REGISTER,
        ADMIN_LOGIN,
        PASSENGER_DASHBOARD,
        ADMIN_DASHBOARD,
        BOOKING_DETAILS,
        FLIGHT_RESULTS,
        ADMIN_FLIGHT_RESULTS,
        ADMIN_UPDATE_FLIGHT,
        ADMIN_ADD_FLIGHT
    };


    ReservationSystem* system;
    ChatBot* chatbott;
    ChatbotWidget* chatbotWidget;
    

    QStackedWidget* stackedWidget;
    
    QLineEdit* passEmail;
    QLineEdit* passpass;
    
    QLineEdit* regName;
    QLineEdit* regEmail;
    QLineEdit* regPhone;
    QSpinBox* regAge;
    QLineEdit* regPass;
    QComboBox* regNational;
    


    QLineEdit* adUser;
    QLineEdit* adPassw;
    

    QLabel* walletLabel;
    QLineEdit* searchSource;
    QLineEdit* searchDest;
    QDateEdit* searchDate;
    QTableWidget* flightTable;
    QTableWidget* ticketTable;
    QVBoxLayout* ticketListLay;
    

    QVBoxLayout* flightListLay;
    

    QLabel* bookInfoLabel;
    QComboBox* seatClassCombo;
    QComboBox* paymentCombo;
    QCheckBox* mealbox;
    QCheckBox* insubox;
    QLabel* totalPriceLabel;
    
 
    QLineEdit* adminSource;
    QLineEdit* adminDest;
    QDateEdit* adminDate;
    QLineEdit* flightNoIn;
    QLineEdit* airlineIn;
    QLineEdit* sourceIn;
    QLineEdit* destIn;
    QLineEdit* depTimeIn;
    QLineEdit* arrTimeIn;
    QDateEdit* flightDateIn;
    QSpinBox* seatsInput;
    QSpinBox* fareInput;
    QComboBox* statusCombo;
    
 
    QVBoxLayout* adminflListLay;
    QLabel* updateFlightInfoLabel;
    QLineEdit* updateDepTime;
    QLineEdit* updateArrTime;
    QSpinBox* updatePrice;
    QComboBox* updateStatusCombo;
    
    
    QString selectedflightNo;
    QString selectedTicketId;
    Flight* selectedFlight;
    double baseFlightPrice;
    



    void setupUI();
    void starter();
    void loginPage();
    void signPage();
    void adminlogScreen();
    void passDash();
    void adminDash();
    void bookPage();
    void flightResPage();
    void adminFlightResPage();
    void adminUpdatePage();

    
  


    void createFlightCard(const Flight& flight);
    void createTicketCard(std::shared_ptr<Ticket> ticket);
    void createAdminFlightCard(const Flight& flight);
    void showBookingScreen(const Flight& flight);
    void showAdminUpdateScreen(const Flight& flight);
    

    void updateFlightTable(const std::vector<Flight>& flights);
    void updateTicketTable();
    void updateAdminFlightTable(const std::vector<Flight>& flights);
    

    void showMessage(const QString& title, const QString& message, bool isError = false);
    void clearForm(QWidget* widget);
    void switchToScreen(int index);
    // void firstStyle();
};

#endif