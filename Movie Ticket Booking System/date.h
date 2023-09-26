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
    Date(int day = 1, int month = 1, int year = 1) : day(day), month(month), year(year) {}
    Date& operator= (const Date& d) {
        if (this) {
            day = d.day;
            month = d.month;
            year = d.year;
        }
        return *this;
    }
    int getYear() const { return year; };
    int getMonth() const { return month; };
    int getDay() const { return day; };
    void readDate() {
        cout << "Year: ";
        cin >> this->year;
        cout << "Month: ";
        cin >> this->month;
        cout << "Day: ";
        cin >> this->day;
    }
    void displayDate() {
        cout << year << "-" << month << "-" << day;
    }
    friend ostream& operator<<(ostream& os, const Date& d);
    friend istream& operator>>(istream& is, Date& d);
};

ostream& operator<<(ostream& os, const Date& d) {
    os << d.year << "-" << d.month << "-" << d.day;
    return os;
}

istream& operator>>(istream& is, Date& d) {
    cout << "Year: ";
    cin >> d.year;
    cout << "Month: ";
    cin >> d.month;
    cout << "Day: ";
    cin >> d.day;

    return is;
}
