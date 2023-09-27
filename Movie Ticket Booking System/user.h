#pragma once

#include <sstream>
#include "address.h"
#include "database_connection.h"
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::cin;
using std::getline;
using std::istringstream;

class User {
protected:
	string lastName;
	string firstName;
	string email;
	string password;
public:
	User(const string& lastName = "", const string& firstName = "", const string& email = "", const string& password = "");
	virtual ~User() {}
	const string getFirstName();
	const string getLastName();
	const string getEmail();
	const string getPassword();
	void setFirstName(string& newFirstName);
	void setLastName(string& newLastName);
	void setEmail(string& newEmail);
	void setPassword(string& newPassword);
};

class UserRepository {
private:
	DatabaseConnector dbConnector;
public:
	UserRepository() {}
	void updateUser(User& user, const string& field, const string& value);
	void listUsersByName(const string& lastName, const string& firstName);
};