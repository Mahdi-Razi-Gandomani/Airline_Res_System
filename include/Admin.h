#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include <string>

using namespace std;


class Admin : public Person {
private:
    string username;
    string password;

public:
    Admin();
    Admin(const string& id, const string& name,const string& email, const string& phone, int age, const string& username, const string& password);


    string getUsername() const {return username;}
    string getPassword() const {return password;}

    void setUsername(const string& user) {username = user;}
    void setPassword(const string& pass) {password = pass;}

 
    bool auth(const string& user, const string& pass) const;

    // Override
    void displayInfo() const override;
    string getRole() const override {return "Admin";}

};

#endif