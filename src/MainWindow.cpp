#include "MainWindow.h"
#include <QInputDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QFont>
#include <QPalette>
#include <QApplication>
using namespace std;


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    system = ReservationSystem::getInstance();
    system->loadAllData();
    
    if (system->getAllFlights().empty()) {
        system->initSampleData();
    }

    chatbott = new ChatBot(system, this);
    
    setupUI();
    // firstStyle();
    
    setWindowTitle("Airline Reservation System");
    setMinimumSize(1200, 800);
    resize(1400, 900);
}


MainWindow::~MainWindow() {
    
    system->saveAllData();
    delete chatbott;

}
void MainWindow::setupUI() {
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    starter();
    loginPage();
    signPage();
    adminlogScreen();
    passDash();
    adminDash();
    bookPage();
    flightResPage();
    adminFlightResPage();
    adminUpdatePage();
    // adminAddPage();
    stackedWidget->setCurrentIndex(WELCOME);
}


void MainWindow::starter() {
    QWidget* welcomeWidget = new QWidget();


    QVBoxLayout* mainLayout = new QVBoxLayout(welcomeWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(30);

    // Card container
    QGroupBox* cardBox = new QGroupBox();
    cardBox->setMaximumWidth(450);
    cardBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:40px;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(cardBox);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(25);


    QLabel* titleLabel = new QLabel("AIRLINE RESERVATION SYSTEM");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size:20px; font-weight:800; color:#1a1a1a;");
    layout->addWidget(titleLabel);

    // Buttons style
    QString buttonStyle =
        "QPushButton {"
        "height:50px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:12px;"
        "background: #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }";

    QPushButton* passengerBtn = new QPushButton("Passenger Login");
    QPushButton* registerBtn = new QPushButton("New Passenger Registration");
    QPushButton* adminBtn = new QPushButton("Admin Portal");
    QPushButton* exitBtn = new QPushButton("Exit");

    passengerBtn->setStyleSheet(buttonStyle);
    registerBtn->setStyleSheet(buttonStyle);
    adminBtn->setStyleSheet(buttonStyle);

    exitBtn->setStyleSheet(
        "QPushButton {"
        "height:50px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:12px;"
        "color:#555;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );

    passengerBtn->setFixedHeight(50);
    registerBtn->setFixedHeight(50);
    adminBtn->setFixedHeight(50);
    exitBtn->setFixedHeight(50);

    // Connect to functions
    connect(passengerBtn, &QPushButton::clicked, this, [this]() {
        switchToScreen(PASSENGER_LOGIN);
    });
    connect(registerBtn, &QPushButton::clicked, this, [this]() {
        switchToScreen(PASSENGER_REGISTER);
    });
    connect(adminBtn, &QPushButton::clicked, this, [this]() {
        switchToScreen(ADMIN_LOGIN);
    });

    connect(exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);



    layout->addWidget(passengerBtn);
    layout->addWidget(registerBtn);
    layout->addWidget(adminBtn);
    layout->addWidget(exitBtn);
    mainLayout->addWidget(cardBox);
    stackedWidget->addWidget(welcomeWidget);
}



void MainWindow::loginPage() {
    QWidget* loginWidget = new QWidget();


    QVBoxLayout* mainLayout = new QVBoxLayout(loginWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Card container
    QGroupBox* loginBox = new QGroupBox();
    loginBox->setMaximumWidth(420);
    loginBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:40px;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(loginBox);


    QLabel* heading = new QLabel("Welcome Back");
    heading->setAlignment(Qt::AlignCenter);
    heading->setStyleSheet("font-size:32px; font-weight:800; color:#1a1a1a;");
    layout->addWidget(heading);

    QLabel* sub = new QLabel("Login to your account");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("font-size:14px; color:#777;");
    layout->addWidget(sub);

    layout->addSpacing(15);

 
    QString inputStyle =
        "QLineEdit {"
        "height:45px;"
        "padding-left:12px;"
        "border-radius:8px;"
        "border:2px solid #e0e0e0;"
        "background:white;"
        "font-size:14px;"
        "}"
        "QLineEdit:focus {"
        "border:2px solid #6a85ff;"
        "}";

    passEmail = new QLineEdit();
    passEmail->setPlaceholderText("Email");
    passEmail->setStyleSheet(inputStyle);
    layout->addWidget(passEmail);

    passpass = new QLineEdit();
    passpass->setPlaceholderText("Passport number");
    passpass->setStyleSheet(inputStyle);
    layout->addWidget(passpass);

    layout->addSpacing(20);

    QPushButton* loginBtn = new QPushButton("Login");
    loginBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:15px;"
        "background: #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }"
    );

    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onlogin);

    layout->addWidget(loginBtn);


    QPushButton* backBtn = new QPushButton(" Back");
    backBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:15px;"
        "color:#555;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );

    connect(backBtn, &QPushButton::clicked, this, [this]() { switchToScreen(WELCOME); });
    layout->addWidget(backBtn);


    mainLayout->addWidget(loginBox);
    stackedWidget->addWidget(loginWidget);
}



void MainWindow::signPage() {
    QWidget* registerWidget = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout(registerWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Card container
    QGroupBox* registerBox = new QGroupBox();
    registerBox->setMaximumWidth(420);
    registerBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:40px;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(registerBox);


    QLabel* heading = new QLabel("Create Account");
    heading->setAlignment(Qt::AlignCenter);
    heading->setStyleSheet("font-size:32px; font-weight:800; color:#1a1a1a;");
    layout->addWidget(heading);

    QLabel* sub = new QLabel("Register a new account");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("font-size:14px; color:#777;");
    layout->addWidget(sub);

    layout->addSpacing(15);


    QString inputStyle =
        "QLineEdit, QComboBox, QSpinBox {"
        "height:45px;"
        "padding-left:12px;"
        "border-radius:8px;"
        "border:2px solid #e0e0e0;"
        "background:white;"
        "font-size:14px;"
        "}"
        "QLineEdit:focus, QComboBox:focus, QSpinBox:focus {"
        "border:2px solid #6a85ff;"
        "}";


    regName = new QLineEdit();
    regName->setPlaceholderText("Full Name");
    regName->setStyleSheet(inputStyle);
    layout->addWidget(regName);

    regEmail = new QLineEdit();
    regEmail->setPlaceholderText("Email");
    regEmail->setStyleSheet(inputStyle);
    layout->addWidget(regEmail);

    regPhone = new QLineEdit();
    regPhone->setPlaceholderText("Phone Number");
    regPhone->setStyleSheet(inputStyle);
    layout->addWidget(regPhone);

    regAge = new QSpinBox();
    regAge->setRange(1, 120);
    regAge->setValue(18);
    regAge->setStyleSheet(inputStyle);
    layout->addWidget(regAge);

    regPass = new QLineEdit();
    regPass->setPlaceholderText("Passport Number");
    regPass->setStyleSheet(inputStyle);
    layout->addWidget(regPass);

    regNational = new QComboBox();
    regNational->addItems({
        "United States", "United Kingdom", "Canada", "Australia",
        "Germany", "France", "Italy", "Spain", "India", "China",
        "Japan", "South Korea", "Brazil", "Mexico", "Argentina",
        "Iran", "Saudi Arabia", "UAE", "Qatar", "Egypt",
        "South Africa", "Nigeria"
    });
    regNational->setStyleSheet(inputStyle);
    layout->addWidget(regNational);
    layout->addSpacing(20);


    QPushButton* registerBtn = new QPushButton("Register");
    registerBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:15px;"
        "background: #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }"
    );

    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::onregister);
    layout->addWidget(registerBtn);


    QPushButton* backBtn = new QPushButton(" Back");
    backBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:15px;"
        "color:#555;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );

    connect(backBtn, &QPushButton::clicked, this, [this]() {switchToScreen(WELCOME);});
    layout->addWidget(backBtn);



    mainLayout->addWidget(registerBox);
    stackedWidget->addWidget(registerWidget);
}


void MainWindow::adminlogScreen() {
    QWidget* adminWidget = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(adminWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Card container
    QGroupBox* loginBox = new QGroupBox();
    loginBox->setMaximumWidth(420);
    loginBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:40px;"
        "}"
    );

    QVBoxLayout* layout = new QVBoxLayout(loginBox);


    QLabel* heading = new QLabel("Admin Login");
    heading->setAlignment(Qt::AlignCenter);
    heading->setStyleSheet("font-size:32px; font-weight:800; color:#1a1a1a;");
    layout->addWidget(heading);

    QLabel* sub = new QLabel("Login to your admin account");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("font-size:14px; color:#777;");
    layout->addWidget(sub);

    layout->addSpacing(15);


    // Input style
    QString inputStyle =
        "QLineEdit {"
        "height:45px;"
        "padding-left:12px;"
        "border-radius:8px;"
        "border:2px solid #e0e0e0;"
        "background:white;"
        "font-size:14px;"
        "}"
        "QLineEdit:focus {"
        "border:2px solid #6a85ff;"
        "}";



    
    adUser = new QLineEdit();
    adUser->setPlaceholderText("Username");
    adUser->setStyleSheet(inputStyle);
    layout->addWidget(adUser);

    adPassw = new QLineEdit();
    adPassw->setPlaceholderText("Password");
    adPassw->setEchoMode(QLineEdit::Password);
    adPassw->setStyleSheet(inputStyle);
    layout->addWidget(adPassw);

    layout->addSpacing(20);

    
    QPushButton* loginBtn = new QPushButton("Login");
    loginBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:15px;"
        "background: #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }"
    );

    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onAdminLogin);

    layout->addWidget(loginBtn);

    // Back button
    QPushButton* backBtn = new QPushButton(" Back");
    backBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:15px;"
        "color:#555;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );

    connect(backBtn, &QPushButton::clicked, this, [this]() {switchToScreen(WELCOME);});
    layout->addWidget(backBtn);

 

    mainLayout->addWidget(loginBox);
    stackedWidget->addWidget(adminWidget);
}



void MainWindow::passDash() {
    QWidget* dashboardWidget = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout(dashboardWidget);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Card container
    QGroupBox* cardBox = new QGroupBox();
    cardBox->setMaximumWidth(900);
    cardBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:20px;"
        "}"
    );
    QVBoxLayout* cardLayout = new QVBoxLayout(cardBox);
    cardLayout->setSpacing(20);

    // Header
    QHBoxLayout* headLay = new QHBoxLayout();
    QLabel* welcomeLabel = new QLabel("Dashboard");
    welcomeLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");

    walletLabel = new QLabel("Wallet: $0.00");
    walletLabel->setStyleSheet("font-size: 18px; color: #27ae60; font-weight: bold;");

    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "height:40px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:14px;"
        "color:#555;"
        "padding:0 10px;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );
    logoutBtn->setMaximumWidth(120);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogout);

    headLay->addWidget(welcomeLabel);
    headLay->addStretch();
    headLay->addWidget(walletLabel);
    headLay->addWidget(logoutBtn);

    cardLayout->addLayout(headLay);

    // Tabs
    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->setStyleSheet(
        "QTabWidget::pane { border: none; }"
        "QTabBar::tab { padding: 10px 20px; font-size: 14px; }"
        "QTabBar::tab:selected { font-weight: bold; }"
    );




    //////
    QWidget* searchTab = new QWidget();
    QVBoxLayout* seachLay = new QVBoxLayout(searchTab);
    seachLay->setAlignment(Qt::AlignCenter);
    
    QGroupBox* searchBox = new QGroupBox();
    searchBox->setMaximumWidth(500);
    searchBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:2px solid #e0e0e0;"
        "padding:30px;"
        "}"
    );
    
    QVBoxLayout* searchFormLay = new QVBoxLayout(searchBox);
    QLabel* seachHead = new QLabel("Search Flights");
    seachHead->setAlignment(Qt::AlignCenter);
    seachHead->setStyleSheet("font-size:24px; font-weight:800; color:#1a1a1a;");
    searchFormLay->addWidget(seachHead);
    
    // QLabel* searchSub = new QLabel("???");
    // searchSub->setAlignment(Qt::AlignCenter);
    // searchSub->setStyleSheet("font-size:14px; color:#777;");
    // searchFormLay->addWidget(searchSub);
    
    searchFormLay->addSpacing(20);
    
    // Input style
    QString inputStyle =
        "QLineEdit, QDateEdit {"
        "height:45px;"
        "padding-left:12px;"
        "border-radius:8px;"
        "border:2px solid #e0e0e0;"
        "background:white;"
        "font-size:14px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus {"
        "border:2px solid #6a85ff;"
        "}";
    

    // From
    searchFormLay->addWidget(new QLabel("From:"));
    searchSource = new QLineEdit();
    searchSource->setPlaceholderText("Departure City");
    searchSource->setStyleSheet(inputStyle);
    searchFormLay->addWidget(searchSource);
    
    // To
    searchFormLay->addWidget(new QLabel("To:"));
    searchDest = new QLineEdit();
    searchDest->setPlaceholderText("Destination City");
    searchDest->setStyleSheet(inputStyle);
    searchFormLay->addWidget(searchDest);
    
    // Date
    searchFormLay->addWidget(new QLabel("Date:"));
    searchDate = new QDateEdit();
    searchDate->setDate(QDate::currentDate());
    searchDate->setCalendarPopup(true);
    searchDate->setStyleSheet(inputStyle);
    searchFormLay->addWidget(searchDate);
    
    searchFormLay->addSpacing(20);
    
    // Search button
    QPushButton* searchBtn = new QPushButton("Search Flights");
    searchBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:15px;"
        "background:  #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }"
    );

    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onSearchFlights);
    searchFormLay->addWidget(searchBtn);
    

    seachLay->addWidget(searchBox);



    //////////
    QWidget* ticketsTab = new QWidget();
    QVBoxLayout* ticketsLay = new QVBoxLayout(ticketsTab);
    ticketsLay->setAlignment(Qt::AlignTop);
    ticketsLay->setSpacing(20);
    ticketsLay->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel* ticketsTitle = new QLabel("My Booked Flights");
    ticketsTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    ticketsTitle->setAlignment(Qt::AlignCenter);
    ticketsLay->addWidget(ticketsTitle);

    // scrol??
    QScrollArea* scrollTicks = new QScrollArea();
    scrollTicks->setWidgetResizable(true);
    scrollTicks->setStyleSheet("border: none; background: transparent;");

    QWidget* ticketContainer = new QWidget();
    ticketListLay = new QVBoxLayout(ticketContainer);
    ticketListLay->setSpacing(15);
    ticketListLay->setAlignment(Qt::AlignTop);
    ticketListLay->setContentsMargins(0, 0, 0, 0);

    scrollTicks->setWidget(ticketContainer);
    ticketsLay->addWidget(scrollTicks);



///////////// ChatBot
    QWidget* chatTab = new QWidget();
    QVBoxLayout* chatLay = new QVBoxLayout(chatTab);
    chatLay->setAlignment(Qt::AlignCenter);
    chatLay->setContentsMargins(20, 20, 20, 20);
    
    // Card container
    QGroupBox* chatBox = new QGroupBox();
    chatBox->setMaximumWidth(900);
    chatBox->setMaximumHeight(700);
    chatBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:2px solid #e0e0e0;"
        "padding:0px;"
        "}"
    );
    
    QVBoxLayout* chatBoxLayout = new QVBoxLayout(chatBox);
    chatBoxLayout->setSpacing(0);
    chatBoxLayout->setContentsMargins(0, 0, 0, 0);
    

    QWidget* titleBar = new QWidget();
    titleBar->setStyleSheet("background: #2e5bff; border-radius: 20px 20px 0 0;");
    QHBoxLayout* titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel* chatTitle = new QLabel("AI Assistant");
    chatTitle->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    titleLayout->addWidget(chatTitle);
    
    chatBoxLayout->addWidget(titleBar);
    
    // Chatbot widget
    chatbotWidget = new ChatbotWidget(chatbott);
    chatBoxLayout->addWidget(chatbotWidget);
    chatLay->addWidget(chatBox);
    



    // All tabs
    tabWidget->addTab(searchTab, "Search & Book");
    tabWidget->addTab(ticketsTab, "My Tickets");
    tabWidget->addTab(chatTab, "AI Assistant");
    

    cardLayout->addWidget(tabWidget);
    mainLayout->addWidget(cardBox);
    stackedWidget->addWidget(dashboardWidget);
}




void MainWindow::adminDash() {
    QWidget* adminWidget = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout(adminWidget);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Card container
    QGroupBox* cardBox = new QGroupBox();
    cardBox->setMaximumWidth(900);
    cardBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:20px;"
        "}"
    );

    QVBoxLayout* cardLayout = new QVBoxLayout(cardBox);
    cardLayout->setSpacing(20);

    // Header
    QHBoxLayout* headLay = new QHBoxLayout();
    QLabel* titleLabel = new QLabel("Admin Dashboard");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color:#1a1a1a;");

    QPushButton* logoutBtn = new QPushButton("Logout");
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "height:40px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:14px;"
        "color:#555;"
        "padding:0 10px;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );
    logoutBtn->setMaximumWidth(120);
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogout);

    headLay->addWidget(titleLabel);
    headLay->addStretch();
    headLay->addWidget(logoutBtn);
    cardLayout->addLayout(headLay);

    // Tabs
    QTabWidget* tabWidget = new QTabWidget();
    tabWidget->setStyleSheet(
        "QTabWidget::pane { border: none; }"
        "QTabBar::tab { padding: 10px 20px; font-size: 14px; }"
        "QTabBar::tab:selected { font-weight: bold; }"
    );

    // Search to update
    QWidget* searchTab = new QWidget();
    QVBoxLayout* seachLay = new QVBoxLayout(searchTab);
    seachLay->setAlignment(Qt::AlignCenter);
    
    QGroupBox* searchBox = new QGroupBox();
    searchBox->setMaximumWidth(500);
    searchBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:2px solid #e0e0e0;"
        "padding:30px;"
        "}"
    );
    
    QVBoxLayout* searchFormLay = new QVBoxLayout(searchBox);
    
    QLabel* seachHead = new QLabel("Search Flights");
    seachHead->setAlignment(Qt::AlignCenter);
    seachHead->setStyleSheet("font-size:24px; font-weight:800; color:#1a1a1a;");
    searchFormLay->addWidget(seachHead);
    
    searchFormLay->addSpacing(20);
    

    QString inputStyle =
        "QLineEdit, QDateEdit , QSpinBox, QComboBox  {"
        "height:45px;"
        "padding-left:12px;"
        "border-radius:8px;"
        "border:2px solid #e0e0e0;"
        "background:white;"
        "font-size:14px;"
        "}"
        "QLineEdit:focus, QDateEdit:focus {"
        "border:2px solid #6a85ff;"
        "}";
    
    searchFormLay->addWidget(new QLabel("From:"));
    adminSource = new QLineEdit();
    adminSource->setPlaceholderText("Departure City");
    adminSource->setStyleSheet(inputStyle);
    searchFormLay->addWidget(adminSource);
    
    searchFormLay->addWidget(new QLabel("To:"));
    adminDest = new QLineEdit();
    adminDest->setPlaceholderText("Destination City");
    adminDest->setStyleSheet(inputStyle);
    searchFormLay->addWidget(adminDest);
    
    searchFormLay->addWidget(new QLabel("Date:"));
    adminDate = new QDateEdit();
    adminDate->setDate(QDate::currentDate());
    adminDate->setCalendarPopup(true);
    adminDate->setStyleSheet(inputStyle);
    searchFormLay->addWidget(adminDate);
    
    searchFormLay->addSpacing(20);
    
    QPushButton* searchBtn = new QPushButton("Search Flights");
    searchBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:15px;"
        "background:  #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }"
    );
    connect(searchBtn, &QPushButton::clicked, this, &MainWindow::onAdminSearch);
    searchFormLay->addWidget(searchBtn);
    
    seachLay->addWidget(searchBox);

    // Add Flight
    QWidget* addTab = new QWidget();
    QVBoxLayout* addLayout = new QVBoxLayout(addTab);
    addLayout->setAlignment(Qt::AlignCenter);
    
    QGroupBox* addFlightBox = new QGroupBox();
    addFlightBox->setMaximumWidth(800);
    addFlightBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:2px solid #e0e0e0;"
        "padding:30px;"
        "}"
    );
    
    QVBoxLayout* addfLay = new QVBoxLayout(addFlightBox);
    
    QLabel* addHeading = new QLabel("Add New Flight");
    addHeading->setAlignment(Qt::AlignCenter);
    addHeading->setStyleSheet("font-size:24px; font-weight:800; color:#1a1a1a;");
    addfLay->addWidget(addHeading);
    
    addfLay->addSpacing(20);
    

    


    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(15);
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 1);
    
    int row = 0;
    
    flightNoIn = new QLineEdit();
    flightNoIn->setPlaceholderText("Flight Num");
    flightNoIn->setStyleSheet(inputStyle);
    gridLayout->addWidget(flightNoIn, 0, 0);
    
    airlineIn = new QLineEdit();
    airlineIn->setPlaceholderText("Airline");
    airlineIn->setStyleSheet(inputStyle);
    gridLayout->addWidget(airlineIn, 0, 1);
    row += 1;
    
    sourceIn = new QLineEdit();
    sourceIn->setPlaceholderText("Depart City");
    sourceIn->setStyleSheet(inputStyle);
    gridLayout->addWidget(sourceIn, row, 0);
    
    destIn = new QLineEdit();
    destIn->setPlaceholderText("Arrival City");
    destIn->setStyleSheet(inputStyle);
    gridLayout->addWidget(destIn, row, 1);
    row += 1;
    
    depTimeIn = new QLineEdit();
    depTimeIn->setPlaceholderText("Departure Time: e.g., 14:30");
    depTimeIn->setStyleSheet(inputStyle);
    gridLayout->addWidget(depTimeIn, row, 0);
    
    arrTimeIn = new QLineEdit();
    arrTimeIn->setPlaceholderText("Arrival Time: e.g., 18:45");
    arrTimeIn->setStyleSheet(inputStyle);
    gridLayout->addWidget(arrTimeIn, row, 1);
    row += 1;
    

    flightDateIn = new QDateEdit();
    flightDateIn->setDate(QDate::currentDate().addDays(1));
    flightDateIn->setCalendarPopup(true);
    flightDateIn->setStyleSheet(inputStyle);
    gridLayout->addWidget(flightDateIn, row, 0);
    

    seatsInput = new QSpinBox();
    seatsInput->setMaximum(500);
    seatsInput->setStyleSheet(inputStyle);
    gridLayout->addWidget(seatsInput, row, 1);
    row += 1;
    

    fareInput = new QSpinBox();
    fareInput->setValue(250);
    fareInput->setPrefix("$ ");
    fareInput->setMaximum(10000);
    fareInput->setStyleSheet(inputStyle);
    gridLayout->addWidget(fareInput, row, 0);
    
    // Status
    statusCombo = new QComboBox();
    statusCombo->addItems({"Scheduled", "Departed", "Arrived", "Delayed", "Cancelled"});
    statusCombo->setStyleSheet(inputStyle);
    gridLayout->addWidget(statusCombo, row, 1);
    
    addfLay->addLayout(gridLayout);
    addfLay->addSpacing(20);
    
    QPushButton* addFlightBtn = new QPushButton("Add Flight");
    addFlightBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:15px;"
        "background: #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }"
    );

    connect(addFlightBtn, &QPushButton::clicked, this, &MainWindow::onAddFlight);
    addfLay->addWidget(addFlightBtn);
    


    addLayout->addWidget(addFlightBox);

    tabWidget->addTab(searchTab, "Manage Flights");
    tabWidget->addTab(addTab, "Add Flight");

    cardLayout->addWidget(tabWidget);
    mainLayout->addWidget(cardBox);
    stackedWidget->addWidget(adminWidget);

}




void MainWindow::onlogin() {
    QString email = passEmail->text().trimmed();
    QString passport = passpass->text().trimmed();

    if (email.isEmpty() || passport.isEmpty()) {
        showMessage("Error", "Fill in all fields.", true);
        return;
    }

    auto passenger = system->logPass(email.toStdString(), passport.toStdString());
    if (passenger) {
        switchToScreen(PASSENGER_DASHBOARD);
        walletLabel->setText(QString("Wallet: $%1").arg(passenger->getWalletBalance(), 0, 'f', 2));
        updateFlightTable(system->getAllFlights());
        updateTicketTable();
        // showMessage("Success", "Welcome back, " + QString::fromStdString(passenger->getName()));
    } else {
        showMessage("Error", "Invalid info. Please try again.", true);
    }
}

void MainWindow::onregister() {
    QString name = regName->text().trimmed();
    QString email = regEmail->text().trimmed();
    QString phone = regPhone->text().trimmed();
    int age = regAge->value();
    QString passport = regPass->text().trimmed();
    // QString nationality = regNational->text().trimmed();
    QString nationality = regNational->currentText().trimmed();

    if (name.isEmpty() || email.isEmpty() || passport.isEmpty()) {
        showMessage("Error", "Please fill in all fields.", true);
        return;
    }

    auto passenger = system->regPass(
        name.toStdString(), email.toStdString(), phone.toStdString(),
        age, passport.toStdString(), nationality.toStdString()
    );

    if (passenger) {
        // Send welcome email
        
        showMessage("Success", "Signup successful, Please login to continue.");
        switchToScreen(PASSENGER_LOGIN);
        clearForm(stackedWidget->widget(PASSENGER_REGISTER));
    } else {
        showMessage("Error", "Email or passport may already exist.", true);
    }
}

void MainWindow::onAdminLogin() {
    QString username = adUser->text().trimmed();
    QString password = adPassw->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        showMessage("Error", "Please fill in all fields.", true);
        return;
    }

    auto admin = system->loginAdmin(username.toStdString(), password.toStdString());
    if (admin) {
        switchToScreen(ADMIN_DASHBOARD);

    } else {
        showMessage("Error", "Invalid info.", true);
    }
}

void MainWindow::onLogout() {
    system->logoutPass();
    system->logoutAdmin();
    switchToScreen(WELCOME);
    clearForm(stackedWidget->widget(PASSENGER_LOGIN));
    clearForm(stackedWidget->widget(ADMIN_LOGIN));
}

void MainWindow::onSearchFlights() {
    QString source = searchSource->text().trimmed();
    QString dest = searchDest->text().trimmed();
    QString date = searchDate->date().toString("yyyy-MM-dd");

    auto results = system->searchFlights(
        source.toStdString(),
        dest.toStdString(),
        date.toStdString()
    );

    if (results.empty()) {
        showMessage("No Results", "No flights found");
        return;
    }
    
    switchToScreen(FLIGHT_RESULTS);
    
    // Clear existing cards first
    QLayoutItem* item;
    while ((item = flightListLay->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Add new flight cards
    for (const auto& flight : results) {
        if (flight.getAvailableSeats() > 0) {
            createFlightCard(flight);
        }
    }
}

void MainWindow::onBookFlight() {

    auto passenger = system->getThisPass();
    //  booking details
    SeatClass seatClass = static_cast<SeatClass>(seatClassCombo->currentIndex());
    PaymentMethod payment = static_cast<PaymentMethod>(paymentCombo->currentIndex());
    bool hasMeal = mealbox->isChecked();
    bool hasInsurance = insubox->isChecked();


    auto flight = system->getFlightbyNo(selectedflightNo.toStdString());
    // if (!flight) {
    //     showMessage("Error", "Flight not found.", true);
    //     return;
    // }

    auto availSeats = flight->getAvailableSeats(seatClass);
    if (availSeats.empty()) {
        showMessage("Error", "No seats available in selected class.", true);
        return;
    }

    // Use first available seat
    int seatNumber = availSeats[0].seatNumber;
    double price = availSeats[0].price;
    if (hasMeal) price += 25.0;
    if (hasInsurance) price += 50.0;

 
    
    auto ticket = system->bookTicket(selectedflightNo.toStdString(), seatNumber, seatClass, payment, hasMeal, hasInsurance);

    if (ticket) {
        // Send  email

            showMessage("Success", QString("Booking confirmed!\nSeat: %1\nTotal: $%2")
            .arg(seatNumber)
            .arg(price, 0, 'f', 2)
            
        );
        
        walletLabel->setText(QString("Wallet: $%1").arg(passenger->getWalletBalance(), 0, 'f', 2));
        updateFlightTable(system->getAllFlights());
        updateTicketTable();
        switchToScreen(PASSENGER_DASHBOARD); 
    } else {
        showMessage("Error", "Booking failed. Please try again.", true);
    }
}

// void MainWindow::onViewMyTickets() {
//     updateTicketTable();
// }
// void MainWindow::onViewMyTickets() {
    
// }


void MainWindow::onCancelBooking() {
    // if (selectedTicketId.isEmpty()) {
    //     showMessage("Error", "Please select a ticket to cancel.", true);
    //     return;
    // }

    QMessageBox::StandardButton reply = QMessageBox::question( this, "Confirm Cancellation","Are you sure you want to cancel this booking?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes)
        return;


    bool success = system->cancelTicket(selectedTicketId.toStdString());

    if (success) {
        // Refresh wallet
        auto passenger = system->getThisPass();
        walletLabel->setText(
            QString("Wallet: $%1").arg(passenger->getWalletBalance(), 0, 'f', 2)
        );

        // Update the ticket table
        updateTicketTable();

        // send email

        showMessage("Success", "Booking cancelled succesfully. Refund processed.");
    }
    else {
        showMessage("Error", "Cancellation failed.", true);
    }
}



void MainWindow::onAddToWallet() {
    auto passenger = system->getThisPass();
    if (!passenger) return;

    bool ok;
    double amount = QInputDialog::getDouble(
        this, "Add Funds", "Enter amount to add:",
        100.0, 10.0, 10000.0, 2, &ok
    );

    if (ok) {
        passenger->addToWallet(amount);
        walletLabel->setText(QString("Wallet: $%1").arg(passenger->getWalletBalance(), 0, 'f', 2));
        showMessage("Success", QString("$%1 added to wallet successfully!").arg(amount, 0, 'f', 2));
    }
}

void MainWindow::onAddFlight() {
    QString flightNo = flightNoIn->text().trimmed();
    QString airline = airlineIn->text().trimmed();
    QString source = sourceIn->text().trimmed();
    QString dest = destIn->text().trimmed();

    if (flightNo.isEmpty() || airline.isEmpty() || source.isEmpty() || dest.isEmpty()) {
        showMessage("Error", "Please fill in all required fields.", true);
        return;
    }

    Flight newFlight(
        flightNo.toStdString(),
        airline.toStdString(),
        source.toStdString(),
        dest.toStdString(),
        depTimeIn->text().toStdString(),
        arrTimeIn->text().toStdString(),
        flightDateIn->date().toString("yyyy-MM-dd").toStdString(),
        seatsInput->value(),
        // aircraftInput->text().toStdString(),
        fareInput->value()
    );

    if (system->addFlight(newFlight)) {
        showMessage("Success", "Flight added successfully!");
        updateFlightTable(system->getAllFlights());
        clearForm(stackedWidget->widget(ADMIN_DASHBOARD));
    } else {
        showMessage("Error", "Flight number may already exist.", true);
    }
}


// void MainWindow::onUpdateFlightStatus() {
//     // if (selectedflightNo.isEmpty()) {
//     //     showMessage("Error", "Please select a flight to update.", true);
//     //     return;
//     // }

//     auto flight = system->getFlightbyNo(selectedflightNo.toStdString());
//     if (flight) {
//         flight->setStatus(static_cast<FlightStatus>(statusCombo->currentIndex()));
//         showMessage("Success", "Flight status updated!");
//         updateFlightTable(system->getAllFlights());
//     }
// }




void MainWindow::onFlightSelected(int row, int column) {
    Q_UNUSED(column);
    if (row >= 0 && row < flightTable->rowCount()) {
        selectedflightNo = flightTable->item(row, 0)->text();
    }
}

void MainWindow::onTicketSelected(int row, int column) {
    Q_UNUSED(column);
    if (row >= 0 && row < ticketTable->rowCount()) {
        selectedTicketId = ticketTable->item(row, 0)->text();
    }
}

void MainWindow::showMessage(const QString& title, const QString& message, bool isError) {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    
    if (isError) {
        msgBox.setIcon(QMessageBox::Critical);
    } else {
        msgBox.setIcon(QMessageBox::Information);
    }
    
    msgBox.exec();
}



void MainWindow::updateFlightTable(const vector<Flight>& flights) {
    // Clear existing cards
    QLayoutItem* item;
    while ((item = flightListLay->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Add new cards
    for (const auto& flight : flights) {
        if (flight.getAvailableSeats() > 0) {
            createFlightCard(flight);
        }
    }
}

void MainWindow::updateTicketTable() {
    auto passenger = system->getThisPass();
    if (!passenger) return;

    // Clear existing cards
    QLayoutItem* item;
    while ((item = ticketListLay->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    // Add new cards
    for (const auto& ticket : passenger->getBookedTickets()) {
        createTicketCard(ticket);
    }
}

void MainWindow::clearForm(QWidget* widget) {
    if (!widget) return;
    
    QList<QLineEdit*> lineEdits = widget->findChildren<QLineEdit*>();
    for (QLineEdit* edit : lineEdits) {
        edit->clear();
    }
}

void MainWindow::switchToScreen(int index) {
    stackedWidget->setCurrentIndex(index);
}

// void MainWindow::onViewAllPassengers() {
//     
//   
// }getTableStyle




void MainWindow::createFlightCard(const Flight& flight) {
    QGroupBox* card = new QGroupBox();
    card->setFixedHeight(100);
    card->setStyleSheet(
        "QGroupBox {"
        "background: white;"
        "border: 1px solid #e0e0e0;"
        "border-radius: 8px;"
        "padding: 0px;"
        "margin: 0px;"
        "}"
        "QGroupBox:hover {"
        "border: 2px solid #5a79ff;"
        "background: #fafbfc;"
        "}"
    );
    
    QHBoxLayout* cardLayout = new QHBoxLayout(card);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(20, 15, 20, 15);
    
    // Left airline name only
    QLabel* airlineName = new QLabel(QString::fromStdString(flight.getAirline()));
    airlineName->setStyleSheet("font-weight: bold; font-size: 14px; color: #1a1a1a;");
    airlineName->setFixedWidth(150);
    
    // Middle times and route
    QVBoxLayout* flightInfLay = new QVBoxLayout();
    flightInfLay->setSpacing(2);
    flightInfLay->setAlignment(Qt::AlignVCenter);
    
    QHBoxLayout* timesLay = new QHBoxLayout();
    timesLay->setSpacing(40);

    QLabel* depTime = new QLabel(QString::fromStdString(flight.getdepartTime()));
    depTime->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    depTime->setAlignment(Qt::AlignLeft);
    QLabel* arrTime = new QLabel(QString::fromStdString(flight.getArrivalTime()));
    arrTime->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    arrTime->setAlignment(Qt::AlignLeft);

    timesLay->addWidget(depTime);
    timesLay->addWidget(arrTime);
    timesLay->addStretch();

    

    QHBoxLayout* routeLay = new QHBoxLayout();
    routeLay->setSpacing(40);

    QLabel* source = new QLabel(QString::fromStdString(flight.getSource()));
    source->setStyleSheet("font-size: 12px; color: #666;");
    source->setMinimumWidth(depTime->sizeHint().width());
    source->setAlignment(Qt::AlignLeft);


    QLabel* dest = new QLabel(QString::fromStdString(flight.getDestination()));
    dest->setStyleSheet("font-size: 12px; color: #666;");
    dest->setMinimumWidth(arrTime->sizeHint().width());
    dest->setAlignment(Qt::AlignLeft);

    routeLay->addWidget(source);
    routeLay->addWidget(dest);
    routeLay->addStretch();
        
    flightInfLay->addLayout(timesLay);
    flightInfLay->addLayout(routeLay);
    
    // Right price and button
    QVBoxLayout* priceLayout = new QVBoxLayout();
    priceLayout->setSpacing(10);
    priceLayout->setAlignment(Qt::AlignVCenter);
    QLabel* price = new QLabel(QString("US$%1").arg(flight.getBaseFare(), 0, 'f', 0));
    price->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    price->setAlignment(Qt::AlignRight);

    priceLayout->addWidget(price);
    
    // Select button
    QPushButton* selectBtn = new QPushButton("Select");
    selectBtn->setFixedSize(100, 40);
    selectBtn->setStyleSheet(
        "QPushButton {"
        "background: #2e5bff;"
        "color: white;"
        "border-radius: 6px;"
        "font-weight: bold;"
        "font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "background: #1a4fd6;"
        "}"
    );
    
    connect(selectBtn, &QPushButton::clicked, this, [this, flight]() {
        showBookingScreen(flight);
    });
    


    cardLayout->addWidget(airlineName);
    cardLayout->addLayout(flightInfLay, 1);
    cardLayout->addLayout(priceLayout);
    cardLayout->addWidget(selectBtn);

    flightListLay->addWidget(card);
}



void MainWindow::showBookingScreen(const Flight& flight) {
    selectedFlight = new Flight(flight);
    selectedflightNo = QString::fromStdString(flight.getFlightNo());
    baseFlightPrice = flight.getBaseFare();
    
    // Update the flight info label
    bookInfoLabel->setText(
        QString("%1 â†’ %2\nFlight: %3\nDate: %4")
        .arg(QString::fromStdString(flight.getSource()))
        .arg(QString::fromStdString(flight.getDestination()))
        .arg(QString::fromStdString(flight.getFlightNo()))
        .arg(QString::fromStdString(flight.getdepartDate()))
    );
    
    switchToScreen(BOOKING_DETAILS);
    updateBookingPrice();
}


void MainWindow::bookPage() {
    QWidget* bookingWidget = new QWidget();
    
    QVBoxLayout* mainLayout = new QVBoxLayout(bookingWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    
    // Card container
    QGroupBox* cardBox = new QGroupBox();
    cardBox->setMaximumWidth(500);
    cardBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:40px;"
        "}"
    );
    
    QVBoxLayout* layout = new QVBoxLayout(cardBox);
    
   
    QLabel* heading = new QLabel("Complete Your Booking");
    heading->setAlignment(Qt::AlignCenter);
    heading->setStyleSheet("font-size:32px; font-weight:800; color:#1a1a1a;");
    layout->addWidget(heading);
    
    
    bookInfoLabel = new QLabel();
    bookInfoLabel->setAlignment(Qt::AlignCenter);
    bookInfoLabel->setStyleSheet("font-size:14px; color:#777; margin-bottom:20px;");
    layout->addWidget(bookInfoLabel);
    
    layout->addSpacing(20);
    
    // Input style
    QString inputStyle =
        "QComboBox, QCheckBox {"
        "height:45px;"
        "padding-left:12px;"
        "border-radius:8px;"
        "border:2px solid #e0e0e0;"
        "background:white;"
        "font-size:14px;"
        "}"
        "QComboBox:focus {"
        "border:2px solid #6a85ff;"
        "}";
    

    layout->addWidget(new QLabel("Select Class:"));
    seatClassCombo = new QComboBox();
    seatClassCombo->addItems({"Econ", "Business", "First Class"});
    seatClassCombo->setStyleSheet(inputStyle);
    layout->addWidget(seatClassCombo);
    
    layout->addWidget(new QLabel("Payment method:"));
    paymentCombo = new QComboBox();
    paymentCombo->addItems({"Credit Card", "Wallet"});
    paymentCombo->setStyleSheet(inputStyle);
    layout->addWidget(paymentCombo);
    
    layout->addSpacing(10);
    
  
    mealbox = new QCheckBox("Add meal (+$25)");
    insubox = new QCheckBox("Add insurance (+$50)");
    layout->addWidget(mealbox);
    layout->addWidget(insubox);
    
    layout->addSpacing(20);



    // Dynamic price
    totalPriceLabel = new QLabel("Total Price: $0");
    totalPriceLabel->setAlignment(Qt::AlignCenter);
    totalPriceLabel->setStyleSheet("font-size:20px; font-weight:700; color:#1a1a1a; margin:10px;");
    layout->addWidget(totalPriceLabel);


    connect(seatClassCombo, &QComboBox::currentIndexChanged, this, &MainWindow::updateBookingPrice);
    connect(mealbox, &QCheckBox::checkStateChanged, this, &MainWindow::updateBookingPrice);
    connect(insubox, &QCheckBox::checkStateChanged, this, &MainWindow::updateBookingPrice);

    
    QPushButton* confirmBtn = new QPushButton("Confirm Booking");
    confirmBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:15px;"
        "background: #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }"
    );
    connect(confirmBtn, &QPushButton::clicked, this, &MainWindow::onBookFlight);
    layout->addWidget(confirmBtn);
    
    // Back button
    QPushButton* backBtn = new QPushButton(" Back");
    backBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:15px;"
        "color:#555;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );

    connect(backBtn, &QPushButton::clicked, this, [this]() {switchToScreen(PASSENGER_DASHBOARD);});
    layout->addWidget(backBtn);
    
    mainLayout->addWidget(cardBox);
    stackedWidget->addWidget(bookingWidget);
}


void MainWindow::flightResPage() {
    QWidget* resultsWidget = new QWidget();
    
    QVBoxLayout* mainLayout = new QVBoxLayout(resultsWidget);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    

    QGroupBox* cardBox = new QGroupBox();
    cardBox->setMaximumWidth(1000);
    cardBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:20px;"
        "}"
    );
    
    QVBoxLayout* cardLayout = new QVBoxLayout(cardBox);
    cardLayout->setSpacing(15);
    

    QHBoxLayout* headLay = new QHBoxLayout();
    
    QPushButton* backBtn = new QPushButton(" Back to Search");
    backBtn->setStyleSheet(
        "QPushButton {"
        "height:40px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:14px;"
        "color:#555;"
        "padding:0 20px;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        switchToScreen(PASSENGER_DASHBOARD);
    });
    
    // QLabel* resultsTitle = new QLabel("Available Flights");
    // resultsTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    
    headLay->addWidget(backBtn);
    // headLay->addWidget(resultsTitle, 1, Qt::AlignCenter);
    
    cardLayout->addLayout(headLay);
    
    // Scroll area for flight cards
    QScrollArea* flightScrolls = new QScrollArea();
    flightScrolls->setWidgetResizable(true);
    flightScrolls->setStyleSheet("border: none; background: transparent;");
    


    QWidget* flightContainer = new QWidget();
    flightListLay = new QVBoxLayout(flightContainer);
    flightListLay->setSpacing(15);
    flightListLay->setAlignment(Qt::AlignTop);
    flightListLay->setContentsMargins(0, 0, 0, 0);
    

    flightScrolls->setWidget(flightContainer);
    cardLayout->addWidget(flightScrolls);
    
    mainLayout->addWidget(cardBox);
    stackedWidget->addWidget(resultsWidget);
}


void MainWindow::createTicketCard(shared_ptr<Ticket> ticket) {
    auto flight = ticket->getFlight();
    
    QGroupBox* card = new QGroupBox();
    card->setFixedHeight(100);
    card->setStyleSheet(
        "QGroupBox {"
        "background: white;"
        "border: 1px solid #e0e0e0;"
        "border-radius: 8px;"
        "padding: 0px;"
        "margin: 0px;"
        "}"
    );
    
    QHBoxLayout* cardLayout = new QHBoxLayout(card);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(20, 15, 20, 15);
    

    QLabel* airlineName = new QLabel(QString::fromStdString(flight->getAirline()));
    airlineName->setStyleSheet("font-weight: bold; font-size: 14px; color: #1a1a1a;");
    airlineName->setFixedWidth(150);

    QVBoxLayout* flightInfLay = new QVBoxLayout();
    flightInfLay->setSpacing(2);
    flightInfLay->setAlignment(Qt::AlignVCenter);
    
 
    QHBoxLayout* timesLay = new QHBoxLayout();
    timesLay->setSpacing(40);
    
    QLabel* depTime = new QLabel(QString::fromStdString(flight->getdepartTime()));
    depTime->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    depTime->setAlignment(Qt::AlignLeft);
    
    
    QLabel* arrTime = new QLabel(QString::fromStdString(flight->getArrivalTime()));
    arrTime->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    arrTime->setAlignment(Qt::AlignLeft);
    
    timesLay->addWidget(depTime);
    timesLay->addWidget(arrTime);
    timesLay->addStretch();

    QHBoxLayout* routeLay = new QHBoxLayout();
    routeLay->setSpacing(40);
    
    QLabel* source = new QLabel(QString::fromStdString(flight->getSource()));
    source->setStyleSheet("font-size: 12px; color: #666;");
    source->setMinimumWidth(depTime->sizeHint().width());
    source->setAlignment(Qt::AlignLeft);

    
    QLabel* dest = new QLabel(QString::fromStdString(flight->getDestination()));
    dest->setStyleSheet("font-size: 12px; color: #666;");
    dest->setMinimumWidth(arrTime->sizeHint().width());
    dest->setAlignment(Qt::AlignLeft);
    
    routeLay->addWidget(source);
    routeLay->addWidget(dest);
    routeLay->addStretch();
    
    flightInfLay->addLayout(timesLay);
    flightInfLay->addLayout(routeLay);
    


    QVBoxLayout* priceLayout = new QVBoxLayout();
    priceLayout->setSpacing(5);
    priceLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* price = new QLabel(QString("US$%1").arg(ticket->getPrice(), 0, 'f', 0));
    price->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    price->setAlignment(Qt::AlignRight);
    
    QLabel* seatInfo = new QLabel(QString("Seat: %1").arg(ticket->getSeatNumber()));
    seatInfo->setStyleSheet("font-size: 11px; color: #999;");
    seatInfo->setAlignment(Qt::AlignRight);
    
    priceLayout->addWidget(price);
    priceLayout->addWidget(seatInfo);
    
    // Cancel button
    QPushButton* cancelBtn = new QPushButton("Cancel");
    cancelBtn->setFixedSize(100, 40);
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "background: #ff4444;"
        "color: white;"
        "border-radius: 6px;"
        "font-weight: bold;"
        "font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "background: #cc0000;"
        "}"
    );
    
    QString ticketId = QString::fromStdString(ticket->getTicketId());
    connect(cancelBtn, &QPushButton::clicked, this, [this, ticketId]() {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Cancellation", "Are you sure you want to cancel this booking?",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            if (system->cancelTicket(ticketId.toStdString())) {
                auto passenger = system->getThisPass();
                walletLabel->setText(QString("Wallet: $%1").arg(passenger->getWalletBalance(), 0, 'f', 2));
                updateTicketTable();
                showMessage("Success", "Booking cancelled successfully. Refund processed.");
            } else {
                showMessage("Error", "Cancellation failed.", true);
            }
        }
    });
    

    cardLayout->addWidget(airlineName);
    cardLayout->addLayout(flightInfLay, 1);
    cardLayout->addLayout(priceLayout);
    cardLayout->addWidget(cancelBtn);
    
    ticketListLay->addWidget(card);
}



void MainWindow::adminFlightResPage() {
    QWidget* resultsWidget = new QWidget();
    
    QVBoxLayout* mainLayout = new QVBoxLayout(resultsWidget);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    QGroupBox* cardBox = new QGroupBox();
    cardBox->setMaximumWidth(1000);
    cardBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:20px;"
        "}"
    );
    
    QVBoxLayout* cardLayout = new QVBoxLayout(cardBox);
    cardLayout->setSpacing(15);
    
    QHBoxLayout* headLay = new QHBoxLayout();
    
    QPushButton* backBtn = new QPushButton(" Back to Search");
    backBtn->setStyleSheet(
        "QPushButton {"
        "height:40px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:14px;"
        "color:#555;"
        "padding:0 20px;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );

    connect(backBtn, &QPushButton::clicked, this, [this]() {
        switchToScreen(ADMIN_DASHBOARD);
    });
    
    // QLabel* resultsTitle = new QLabel("Available Flights");
    // resultsTitle->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    
    headLay->addWidget(backBtn);
    // headLay->addWidget(resultsTitle, 1, Qt::AlignCenter);
    
    cardLayout->addLayout(headLay);
    
    QScrollArea* flightScrolls = new QScrollArea();
    flightScrolls->setWidgetResizable(true);
    flightScrolls->setStyleSheet("border: none; background: transparent;");
    
    QWidget* flightContainer = new QWidget();
    adminflListLay = new QVBoxLayout(flightContainer);
    adminflListLay->setSpacing(15);
    adminflListLay->setAlignment(Qt::AlignTop);
    adminflListLay->setContentsMargins(0, 0, 0, 0);
    
    flightScrolls->setWidget(flightContainer);
    cardLayout->addWidget(flightScrolls);
    
    mainLayout->addWidget(cardBox);
    stackedWidget->addWidget(resultsWidget);
}



void MainWindow::onAdminSearch() {
    QString source = adminSource->text().trimmed();
    QString dest = adminDest->text().trimmed();
    QString date = adminDate->date().toString("yyyy-MM-dd");

    auto results = system->searchFlights(
        source.toStdString(),
        dest.toStdString(),
        date.toStdString()
    );

    if (results.empty()) {
        showMessage("No Results", "No flights found");
        return;
    }
    
    switchToScreen(ADMIN_FLIGHT_RESULTS);
    updateAdminFlightTable(results);
}

void MainWindow::showAdminUpdateScreen(const Flight& flight) {
    selectedFlight = new Flight(flight);
    selectedflightNo = QString::fromStdString(flight.getFlightNo());
    
    updateFlightInfoLabel->setText(
        QString("Flight: %1 | %2 â†’ %3 | Date: %4")
        .arg(QString::fromStdString(flight.getFlightNo()))
        .arg(QString::fromStdString(flight.getSource()))
        .arg(QString::fromStdString(flight.getDestination()))
        .arg(QString::fromStdString(flight.getdepartDate()))
    );
    
    updateDepTime->setText(QString::fromStdString(flight.getdepartTime()));
    updateArrTime->setText(QString::fromStdString(flight.getArrivalTime()));
    updatePrice->setValue(flight.getBaseFare());
    updateStatusCombo->setCurrentIndex(static_cast<int>(flight.getStatus()));
    
    switchToScreen(ADMIN_UPDATE_FLIGHT);
}

void MainWindow::onUpdateFlight() {
    if (!selectedFlight) return;
    
    auto flight = system->getFlightbyNo(selectedflightNo.toStdString());
    if (!flight) {
        showMessage("Error", "Flight not found.", true);
        return;
    }
    
    flight->setdepartTime(updateDepTime->text().toStdString());
    flight->setArrivalTime(updateArrTime->text().toStdString());
    flight->setBaseFare(updatePrice->value());
    flight->setStatus(static_cast<FlightStatus>(updateStatusCombo->currentIndex()));
    
    showMessage("Success", "Flight updated successfully!");
    switchToScreen(ADMIN_DASHBOARD);
}

void MainWindow::createAdminFlightCard(const Flight& flight) {
    QGroupBox* card = new QGroupBox();
    card->setFixedHeight(100);
    card->setStyleSheet(
        "QGroupBox {"
        "background: white;"
        "border: 1px solid #e0e0e0;"
        "border-radius: 8px;"
        "padding: 0px;"
        "margin: 0px;"
        "}"
        "QGroupBox:hover {"
        "border: 2px solid #5a79ff;"
        "background: #fafbfc;"
        "}"
    );
    
    QHBoxLayout* cardLayout = new QHBoxLayout(card);
    cardLayout->setSpacing(20);
    cardLayout->setContentsMargins(20, 15, 20, 15);
    
    QLabel* airlineName = new QLabel(QString::fromStdString(flight.getAirline()));
    airlineName->setStyleSheet("font-weight: bold; font-size: 14px; color: #1a1a1a;");
    airlineName->setFixedWidth(150);
    
    QVBoxLayout* flightInfLay = new QVBoxLayout();
    flightInfLay->setSpacing(2);
    flightInfLay->setAlignment(Qt::AlignVCenter);
    
    QHBoxLayout* timesLay = new QHBoxLayout();
    timesLay->setSpacing(40);
    
    QLabel* depTime = new QLabel(QString::fromStdString(flight.getdepartTime()));
    depTime->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    depTime->setAlignment(Qt::AlignLeft);
    

    
    QLabel* arrTime = new QLabel(QString::fromStdString(flight.getArrivalTime()));
    arrTime->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    arrTime->setAlignment(Qt::AlignLeft);
    
    timesLay->addWidget(depTime);
    timesLay->addWidget(arrTime);
    timesLay->addStretch();
    
    QHBoxLayout* routeLay = new QHBoxLayout();
    routeLay->setSpacing(40);
    
    QLabel* source = new QLabel(QString::fromStdString(flight.getSource()));
    source->setStyleSheet("font-size: 12px; color: #666;");
    source->setMinimumWidth(depTime->sizeHint().width());
    source->setAlignment(Qt::AlignLeft);
    

    
    QLabel* dest = new QLabel(QString::fromStdString(flight.getDestination()));
    dest->setStyleSheet("font-size: 12px; color: #666;");
    dest->setMinimumWidth(arrTime->sizeHint().width());
    dest->setAlignment(Qt::AlignLeft);
    
    routeLay->addWidget(source);
    routeLay->addWidget(dest);
    routeLay->addStretch();
    
    flightInfLay->addLayout(timesLay);
    flightInfLay->addLayout(routeLay);
    
    QVBoxLayout* priceLayout = new QVBoxLayout();
    priceLayout->setSpacing(10);
    priceLayout->setAlignment(Qt::AlignVCenter);
    
    QLabel* price = new QLabel(QString("US$%1").arg(flight.getBaseFare(), 0, 'f', 0));
    price->setStyleSheet("font-size: 24px; font-weight: bold; color: #1a1a1a;");
    price->setAlignment(Qt::AlignRight);
    
    priceLayout->addWidget(price);
    
    QPushButton* updateBtn = new QPushButton("Update");
    updateBtn->setFixedSize(100, 40);
    updateBtn->setStyleSheet(
        "QPushButton {"
        "background: #2e5bff;"
        "color: white;"
        "border-radius: 6px;"
        "font-weight: bold;"
        "font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "background: #1a4fd6;"
        "}"
    );
    
    connect(updateBtn, &QPushButton::clicked, this, [this, flight]() {
        showAdminUpdateScreen(flight);
    });
    
    cardLayout->addWidget(airlineName);
    cardLayout->addLayout(flightInfLay, 1);
    cardLayout->addLayout(priceLayout);
    cardLayout->addWidget(updateBtn);
    
    adminflListLay->addWidget(card);
}

void MainWindow::updateAdminFlightTable(const vector<Flight>& flights) {
    QLayoutItem* item;
    while ((item = adminflListLay->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    
    for (const auto& flight : flights) {
        createAdminFlightCard(flight);
    }
}



void MainWindow::adminUpdatePage() {
    QWidget* updateWidget = new QWidget();
    
    QVBoxLayout* mainLayout = new QVBoxLayout(updateWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    
    QGroupBox* cardBox = new QGroupBox();
    cardBox->setMaximumWidth(500);
    cardBox->setStyleSheet(
        "QGroupBox {"
        "background:white;"
        "border-radius:20px;"
        "border:none;"
        "padding:40px;"
        "}"
    );
    
    QVBoxLayout* layout = new QVBoxLayout(cardBox);
    
    QLabel* heading = new QLabel("Update Flight Details");
    heading->setAlignment(Qt::AlignCenter);
    heading->setStyleSheet("font-size:32px; font-weight:800; color:#1a1a1a;");
    layout->addWidget(heading);
    
    updateFlightInfoLabel = new QLabel();
    updateFlightInfoLabel->setAlignment(Qt::AlignCenter);
    updateFlightInfoLabel->setStyleSheet("font-size:14px; color:#777; margin-bottom:20px;");
    layout->addWidget(updateFlightInfoLabel);
    
    layout->addSpacing(20);
    
    QString inputStyle =
        "QLineEdit, QSpinBox, QComboBox {"
        "height:45px;"
        "padding-left:12px;"
        "border-radius:8px;"
        "border:2px solid #e0e0e0;"
        "background:white;"
        "font-size:14px;"
        "}"
        "QLineEdit:focus, QSpinBox:focus, QComboBox:focus {"
        "border:2px solid #6a85ff;"
        "}";
    
    layout->addWidget(new QLabel("Departure Time:"));
    updateDepTime = new QLineEdit();
    // updateDepTime->setPlaceholderText("e.g., 14:30");
    updateDepTime->setStyleSheet(inputStyle);
    layout->addWidget(updateDepTime);
    
    layout->addWidget(new QLabel("Arrival Time:"));
    updateArrTime = new QLineEdit();
    // updateArrTime->setPlaceholderText("e.g., 18:45");
    updateArrTime->setStyleSheet(inputStyle);
    layout->addWidget(updateArrTime);
    
    layout->addWidget(new QLabel("Price:"));
    updatePrice = new QSpinBox();
    updatePrice->setPrefix("$ ");
    updatePrice->setMaximum(10000);
    updatePrice->setStyleSheet(inputStyle);
    layout->addWidget(updatePrice);
    
    layout->addWidget(new QLabel("Status:"));
    updateStatusCombo = new QComboBox();
    updateStatusCombo->addItems({"Scheduled", "Departed", "Arrived", "Delayed", "Cancelled"});
    updateStatusCombo->setStyleSheet(inputStyle);
    layout->addWidget(updateStatusCombo);
    
    layout->addSpacing(20);
    
    QPushButton* updateBtn = new QPushButton("Update Flight");
    updateBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "color:white;"
        "font-weight:bold;"
        "font-size:15px;"
        "background: #2e5bff;"
        "}"
        "QPushButton:hover { opacity:0.9; }"
    );
    connect(updateBtn, &QPushButton::clicked, this, &MainWindow::onUpdateFlight);
    layout->addWidget(updateBtn);
    
    QPushButton* backBtn = new QPushButton(" Back");
    backBtn->setStyleSheet(
        "QPushButton {"
        "height:45px;"
        "border-radius:10px;"
        "background:#efefef;"
        "font-size:15px;"
        "color:#555;"
        "}"
        "QPushButton:hover { background:#e3e3e3; }"
    );
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        switchToScreen(ADMIN_FLIGHT_RESULTS);
    });
    layout->addWidget(backBtn);
    
    mainLayout->addWidget(cardBox);
    stackedWidget->addWidget(updateWidget);
}


void MainWindow::updateBookingPrice() {
    if (baseFlightPrice <= 0) return;

    double price = baseFlightPrice;

    QString cls = seatClassCombo->currentText();
    if (cls == "Business") price += 150;
    else if (cls == "First Class") price += 300;

    if (mealbox->isChecked()) price += 25;
    if (insubox->isChecked()) price += 50;

    totalPriceLabel->setText(QString("Total Price: $%1").arg(price, 0, 'f', 2));
}