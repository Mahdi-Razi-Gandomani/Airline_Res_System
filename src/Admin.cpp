#include "Admin.h"
#include <iostream>
#include <sstream>

using namespace std;

Admin::Admin() : Person(), username(""), password("") {}

Admin::Admin(const string& id, const string& name,const string& email, const string& phone, int age,const string& username, const string& password) : 
                Person(id, name, email, phone, age),
                username(username), password(password) {}

bool Admin::auth(const string& user, const string& pass) const {
    return (username == user && password == pass);
}

void Admin::displayInfo() const {
    Person::displayInfo();
    cout << "Username: " << username << "\n";
}
