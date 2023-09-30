#pragma once
#include <string>
#include "database_connection.h"
#define MAX_LOGIN_ATTEMPTS 3
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

class LoginRepository {
private:
	DatabaseConnector dbConnector;
public:
	LoginRepository() {}
	bool validateLogin(Login& login);
};

class LoginInterface {
public:
	bool displayLoginMenu(Login& login, LoginRepository& loginRep);
};