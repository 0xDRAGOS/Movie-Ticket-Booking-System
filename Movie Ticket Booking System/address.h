#pragma once

#include <string>
using std::string;

class Address {
	string country;
	string county;
	string city;
	string street;
	string number;
public:
	Address(const string& country = "", const string& county = "", const string& city = "", const string& street = "", const string& number = "")
		: country(country), county(county), city(city), street(street), number(number) {}

	virtual ~Address() {}
	const string getCountry() { return country; };
	const string getCounty() { return county; };
	const string getCity() { return city; };
	const string getStreet() { return street; };
	const string getNumber() { return number; };
	Address& operator=(const Address& adr) {
		if (this != &adr) {
			this->country = adr.country;
			this->county = adr.county;
			this->city = adr.city;
			this->street = adr.street;
			this->number = adr.number;
		}
		return *this;
	}
};