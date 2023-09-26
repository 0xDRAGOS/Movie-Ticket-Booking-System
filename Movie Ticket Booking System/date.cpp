#include "date.h"

Date::Date(int day, int month, int year)
    : day(day), month(month), year(year) {}

int Date::getYear() const { return year; };
int Date::getMonth() const { return month; };
int Date::getDay() const { return day; };

void Date::readDate() {
    cout << "Year: ";
    cin >> this->year;
    cout << "Month: ";
    cin >> this->month;
    cout << "Day: ";
    cin >> this->day;
}

void Date::displayDate() {
    cout << year << "-" << month << "-" << day;
}

Date& Date::operator= (const Date& d) {
    if (this) {
        day = d.day;
        month = d.month;
        year = d.year;
    }
    return *this;
}

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
