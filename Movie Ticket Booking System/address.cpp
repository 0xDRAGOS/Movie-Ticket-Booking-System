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

istream& operator>>(istream& is, Address& a) {
    cout << "Country: "; cin >> a.country;
	cout << "County: "; cin >> a.county;
    cout << "City: "; cin >> a.city;
    cout << "Street: "; cin >> a.street;
    cout << "Number: "; cin >> a.number;
    return is;
}