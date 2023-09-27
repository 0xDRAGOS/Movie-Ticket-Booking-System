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
	Address(const string& country = "", const string& county = "", const string& city = "", const string& street = "", const string& number = "");
	virtual ~Address() {}
	const string getCountry();
	const string getCounty();
	const string getCity();
	const string getStreet();
	const string getNumber();
	string toString() const;
	Address& operator=(const Address& adr);
};
