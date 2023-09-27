#pragma once
#include <string>
#include "database_connection.h"
using std::string;
using std::cerr;
using std::endl;
using std::cout;
using std::cin;

class Login {
private:
	string inputEmail;
	string inputPassword;
public:
	Login(const string& inputEmail = "", const string& inputPassoword = "");
	virtual ~Login() {}
	const string getInputEmail();
	const string getInputPassword();
};

class LoginInterface {
public:
	void displayLoginMenu(Login& login, LoginRepository& loginRep);
};

class LoginRepository {
private:
	DatabaseConnector dbConnector;
public:
	LoginRepository() {}
	bool validateLogin(Login& login);
};