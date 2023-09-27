#pragma once
#include <string>
#include "database_connection.h"
using std::string;
using std::cerr;
using std::endl;

class Login {
private:
	string inputEmail;
	string inputPassword;
public:
	Login(const string& inputEmail, const string& inputPassoword);
	virtual ~Login() {}
	const string getInputEmail();
	const string getInputPassword();
};

class LoginRepository {
private:
	DatabaseConnector dbConnector;
public:
	LoginRepository() {}
	bool validateLogin(Login& login);
};