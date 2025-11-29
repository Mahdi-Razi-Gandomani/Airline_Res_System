#include "Person.h"

using namespace std;

Person::Person() : id(""), name(""), email(""), phone(""), age(0) {}

Person::Person(const string& id, const string& name, const string& email, const string& phone, int age) :
    id(id), name(name), email(email), phone(phone), age(age) {}

void Person::displayInfo() const {
    cout << "ID: " << id << "\n" << "Name: " << name << "\n" << "Email: " << email << "\n"
              << "Phone: " << phone << "\n" << "Age: " << age << "\n";
}