#include "MainWindow.h"
#include <iostream>
#include <QApplication>
#include <QMessageBox>

using namespace std;


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("Airline Reservation System");
    
    try {
        MainWindow window;
        window.show();
        
        int result = app.exec();
        
        auto sys = ReservationSystem::getInstance();
        sys->saveAllData();
        
        return result;
    } catch (const exception& e) {
        return 1;
    }
}