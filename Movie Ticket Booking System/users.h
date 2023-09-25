#pragma once

#include <sstream>
#include "address.h"
#include "database_connection.h"
using namespace std;

class User {
protected:
	string lastName;
	string firstName;
	string email;
	string password;
public:
	User(const string& lastName = "", const string& firstName = "", const string& email = "", const string& password = "")
		:  lastName(lastName), firstName(firstName), email(email), password(password){}
	virtual ~User() {}
	const string getFirstName() { return firstName; };
	const string getLastName() { return lastName; };
	const string getEmail() { return email; };
	const string getPassword() { return password; };
	void setFirstName(string& newFirstName) { this->firstName = newFirstName; }
	void setLastName(string& newLastName) { this->lastName = newLastName; }
	void setEmail(string& newEmail) { this->email = newEmail; }
	void setPassword(string& newPassword) { this->password = newPassword; }
};

class UserRepository {
private:
	DatabaseConnector dbConnector;
public:
	UserRepository() {}
	void updateUser(User& user, const string& field, const string& value);
	void listUsersByName(const string& lastName, const string& firstName);
};

void UserRepository::updateUser(User& user, const string& field, const string& value) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		const string query = "UPDATE users SET " + field + " = ? WHERE email = ? AND password = ?";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setString(1, value);
		pstmt->setString(2, user.getEmail());
		pstmt->setString(3, user.getPassword());
		int rowsUpdated = pstmt->executeUpdate();

		if (rowsUpdated > 0) {
			cout << "User " << user.getFirstName() << " " << user.getLastName() << " modified with success." << endl;
			con->commit();
		}
		else {
			cout << "User " << user.getFirstName() << " " << user.getLastName() << " not found." << endl;
		}
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not update user ( " << user.getFirstName() << " " << user.getLastName() << "). Error message : " << e.what() << endl;
		con->rollback();
		exit(1);
	}
	dbConnector.closeConnection(con);
}

void UserRepository::listUsersByName(const string& lastName, const string& firstName) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id, lastName, firstName, email FROM users WHERE lastName = ? AND firstName = ?;");
		pstmt->setString(1, lastName);
		pstmt->setString(2, firstName);
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()) {
			cout << "ID: " << res->getInt("id") << "; Last name: " << res->getString("lastName") << "; First name: " << res->getString("firstName") << "; Email: " << res->getString("email") << endl;
		}
		delete pstmt;
		delete res;
	}catch (sql::SQLException& e) {
			cerr << "Could not list users by name. Error message : " << e.what() << endl;
			exit(1);
		}
	dbConnector.closeConnection(con);
}

class PublicUser : public User {
private:
	Address address;
public:
	PublicUser(const string& lastName = "", const string& firstName = "", const string& email = "", const string& password = "", const Address& address = Address()):
		address(address), User(lastName, firstName, email, password) {}
	const Address getAddress() { return address; };
	void setAddress(Address& newAddress) { address = newAddress; };
};

class PublicUserRepository {
private:
	DatabaseConnector dbConnector;
public:
	PublicUserRepository() {}
	PublicUser loadPublicUser(const string& inputEmail, const string& inputPassword);
};

PublicUser PublicUserRepository::loadPublicUser(const string& inputEmail, const string& inputPassword) {
	PublicUser user;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM users WHERE email = ? AND password = ?;");
		pstmt->setString(1, inputEmail);
		pstmt->setString(2, inputPassword);
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()) {
			string addressData = res->getString("address");
			string country, city, street, number;
			cout << country << city << street << number;
			istringstream iss(addressData);
			getline(iss, country, ',');
			getline(iss, city, ',');
			getline(iss, street, ',');
			getline(iss, number, ',');
			user = PublicUser(res->getString("email"),
				res->getString("firstName"),
				res->getString("lastName"),
				res->getString("password"),
				Address(country, city, street, number));
		}
		cout << "User loaded up successfully." << endl;
		delete pstmt;
		delete res;
	}
		catch (sql::SQLException& e) {
			cerr << "Could not load user. Error: " << e.what() << endl;
			exit(1);
		}
		return user;
	}
