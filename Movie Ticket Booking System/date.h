#pragma once
#include <iostream>
#include <windows.h>
using std::cout;
using std::cin;
using std::istream;
using std::ostream;

class Date {
private:
    int day;
    int month;
    int year;
    int hour;
    int minute;
public:
    Date(int day = 1, int month = 1, int year = 1, int hour = 0, int minute = 0);
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int getHour() const;
    int getMinute() const;
    int getLocalTimeDayOfWeek() const;
    int getLocalTimeHour() const;
    void readDate();
    void displayDate();
    void displayDateHoursMinutes();
    Date& operator= (const Date& d);
    friend ostream& operator<<(ostream& os, const Date& d);
    friend istream& operator>>(istream& is, Date& d);
};