#pragma once
#include <iostream>
#include <string>
using std::string;
using std::istream;
using std::cin;
using std::cout;

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
	friend istream& operator>>(istream& is, Address& a);
};
