#pragma once
#include "public_user.h"

class PrivateUser : public User {
public:
	PrivateUser(const string& lastName = "", const string& firstName = "", const string& email = "", const string& password = "");
};

class PrivateUserRepository {
private:
	DatabaseConnector dbConnector;
public:
	PrivateUserRepository() {}
	PrivateUser loadPrivateUser(const string& inputEmail, const string& inputPassword);
};