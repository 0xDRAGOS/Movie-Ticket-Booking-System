#include "private_user.h"

PrivateUser::PrivateUser(const string& lastName = "", const string& firstName = "", const string& email = "", const string& password = "")
	: User(lastName, firstName, email, password) {}

PrivateUser PrivateUserRepository::loadPrivateUser(const string& inputEmail, const string& inputPassword) {
	PrivateUser user;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM users WHERE user_type = 'private' AND email = ? AND password = ?;");
		pstmt->setString(1, inputEmail);
		pstmt->setString(2, inputPassword);
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()) {
			user = PrivateUser(res->getString("lastName"),
				res->getString("firstName"),
				res->getString("email"),
				res->getString("password")
			);
		}
		cout << "User loaded up successfully." << endl;
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not load user. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return user;
}