#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>
using namespace std;


class Person {
protected:
    string id;
    string name;
    string email;
    string phone;
    int age;

public:
    Person();
    Person(const string& id, const string& name, const string& email, const string& phone, int age);
    virtual ~Person() = default;

    string getId() const {return id;}
    string getName() const {return name;}
    string getEmail() const {return email;}
    string getPhone() const {return phone;}
    int getAge() const {return age;}

    void setId(const string& id) {this->id = id;}
    void setName(const string& name) {this->name = name;}
    void setEmail(const string& email) {this->email = email;}
    void setPhone(const string& phone) {this->phone = phone;}
    void setAge(int age) {this->age = age;}

    virtual void displayInfo() const;
    virtual string getRole() const = 0;
};
#endif