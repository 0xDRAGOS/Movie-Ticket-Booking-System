#pragma once

#include <string>
using namespace std;

class Address {
	string country;
	string city;
	string street;
	int number;
public:
	Address(const string& country, const string& city, const string& street, const int& number)
		: country(country), city(city), street(street), number(number) {}

	virtual ~Address() {}
	Address& operator=(const Address& adr) {
		if (this != &adr) {
			this->country = adr.country;
			this->city = adr.city;
			this->street = adr.street;
			this->number = adr.number;
		}
		return *this;
	}
};