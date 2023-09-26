#pragma once

#include <iostream>
using std::cout;
using std::cin;
using std::istream;
using std::ostream;

class Date {
private:
    int day;
    int month;
    int year;
public:
    Date(int day = 1, int month = 1, int year = 1);
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    void readDate();
    void displayDate();
    Date& operator= (const Date& d);
    friend ostream& operator<<(ostream& os, const Date& d);
    friend istream& operator>>(istream& is, Date& d);
};