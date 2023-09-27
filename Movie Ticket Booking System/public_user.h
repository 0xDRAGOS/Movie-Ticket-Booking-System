#pragma once
#include "user.h"

class PublicUser : public User {
private:
	Address address;
public:
	PublicUser(const string& lastName = "", const string& firstName = "", const string& email = "", const string& password = "", const Address& address = Address());
	const Address getAddress();
	void setAddress(Address& newAddress);
	void read();
};

class PublicUserRepository {
private:
	DatabaseConnector dbConnector;
public:
	PublicUserRepository() {}
	int getPublicUserID(PublicUser& user);
	void updateAddress(PublicUser& user, Address& newAddress);
	PublicUser loadPublicUser(const string& inputEmail, const string& inputPassword);
	void insertIntoDatabase(PublicUser& user);
};