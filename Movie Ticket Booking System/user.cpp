#include "user.h"

User::User(const string& lastName = "", const string& firstName = "", const string& email = "", const string& password = "")
	: lastName(lastName), firstName(firstName), email(email), password(password) {}

const string User::getFirstName() { return firstName; };
const string User::getLastName() { return lastName; };
const string User::getEmail() { return email; };
const string User::getPassword() { return password; };

void User::setFirstName(string& newFirstName) { this->firstName = newFirstName; }
void User::setLastName(string& newLastName) { this->lastName = newLastName; }
void User::setEmail(string& newEmail) { this->email = newEmail; }
void User::setPassword(string& newPassword) { this->password = newPassword; }

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
	}
	catch (sql::SQLException& e) {
		cerr << "Could not list users by name. Error message : " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}
