#include "address.h"

Address::Address(const string& country, const string& county, const string& city, const string& street, const string& number)
: country(country), county(county), city(city), street(street), number(number) {}

string Address::toString() const {
	return country + ", " + county + ", " + city + ", " + street + ", " + number;
}

const string Address::getCountry() { return country; };
const string Address::getCounty() { return county; };
const string Address::getCity() { return city; };
const string Address::getStreet() { return street; };
const string Address::getNumber() { return number; };

Address& Address::operator=(const Address& adr) {
	if (this != &adr) {
		this->country = adr.country;
		this->county = adr.county;
		this->city = adr.city;
		this->street = adr.street;
		this->number = adr.number;
	}
	return *this;
}