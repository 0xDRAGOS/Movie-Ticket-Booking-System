#include "date.h"

Date::Date(int day, int month, int year, int hour, int minute)
    : day(day), month(month), year(year), hour(hour), minute(minute){}

int Date::getYear() const { return year; };
int Date::getMonth() const { return month; };
int Date::getDay() const { return day; };
int Date::getMinute() const { return minute; };
int Date::getHour() const { return hour; };
int Date::getLocalTimeDayOfWeek() const {
    SYSTEMTIME lt;
    GetLocalTime(&lt);
    return lt.wDayOfWeek;
}
int Date::getLocalTimeHour() const {
    SYSTEMTIME lt;
    GetLocalTime(&lt);
    return lt.wHour;
}


void Date::readDate() {
    cout << "Year: ";
    cin >> this->year;
    cout << "Month: ";
    cin >> this->month;
    cout << "Day: ";
    cin >> this->day;
    cout << "Minute: ";
    cin >> this->minute;
    cout << "Hour: ";
    cin >> this->hour;
}

void Date::displayDate() {
    cout << year << "-" << month << "-" << day << " " << hour << ":";
    if (minute == 0)
        cout << "00";
    else
        cout<< minute;
}

void Date::displayDateHoursMinutes() {
    cout << hour << ":";
    if (minute == 0)
        cout << "00";
    else
        cout << minute;
}

Date& Date::operator= (const Date& d) {
    if (this) {
        day = d.day;
        month = d.month;
        year = d.year;
        hour = d.hour;
        minute = d.minute;
    }
    return *this;
}

ostream& operator<<(ostream& os, const Date& d) {
    os << d.year << "-" << d.month << "-" << d.day << " " << d.hour << ":";
    if (d.minute == 0)
        os << "00";
    else
        os << d.minute;
    return os;
}

istream& operator>>(istream& is, Date& d) {
    cout << "Year: ";
    cin >> d.year;
    cout << "Month: ";
    cin >> d.month;
    cout << "Day: ";
    cin >> d.day;
    cout << "Minute: ";
    cin >> d.minute;
    cout << "Hour: ";
    cin >> d.hour;
    return is;
}
