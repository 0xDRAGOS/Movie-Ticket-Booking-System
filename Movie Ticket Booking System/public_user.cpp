#include "public_user.h"

PublicUser::PublicUser(const string& lastName, const string& firstName, const string& email, const string& password, const Address& address) :
	address(address), User(lastName, firstName, email, password) {}
const Address PublicUser::getAddress() { return address; };
void PublicUser::setAddress(Address& newAddress) { address = newAddress; };

void PublicUser::read() {
	cout << "Enter first name: ";
	cin >> firstName;
	cout << "Enter last name: ";
	cin >> lastName;
	cout << "Enter email: ";
	cin >> email;
	cout << "Enter password: ";
	cin >> password;
	string country, county, city, street, number;
	cout << "Enter country: ";
	cin.ignore();
	getline(cin, country);
	cout << "Enter county: ";
	getline(cin, county);
	cout << "Enter city: ";
	getline(cin, city);
	cout << "Enter street: ";
	getline(cin, street);
	cout << "Enter number: ";
	getline(cin, number);
	Address newAddress(country, county, city, street, number);
	setAddress(newAddress);
}

int PublicUserRepository::getPublicUserID(PublicUser& user) {
	sql::Connection* con = dbConnector.establishConnection();
	int id;
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id FROM users WHERE email = ? AND password = ?;");
		pstmt->setString(1, user.getEmail());
		pstmt->setString(2, user.getPassword());
		sql::ResultSet* res = pstmt->executeQuery();
		if (res->next()) {
			id = res->getInt("id");
		}
		else cout << "No data found" << endl;
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get public user id. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return id;
}



void PublicUserRepository::updateAddress(PublicUser& user, Address& newAddress) {
	sql::Connection* con = dbConnector.establishConnection();
	string addressString = newAddress.getCountry() + ", " + newAddress.getCounty() + ", " + newAddress.getCity() + ", " + newAddress.getStreet() + ", " + newAddress.getNumber();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE users SET address = ? WHERE email = ? AND password = ?;");
		pstmt->setString(1, addressString);
		pstmt->setString(2, user.getEmail());
		pstmt->setString(3, user.getPassword());
		pstmt->executeUpdate();
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not update user's address. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

PublicUser PublicUserRepository::loadPublicUser(const string& inputEmail, const string& inputPassword) {
	PublicUser user;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM users WHERE user_type = 'public' AND email = ? AND password = ?;");
		pstmt->setString(1, inputEmail);
		pstmt->setString(2, inputPassword);
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()) {
			string addressData = res->getString("address");
			string country, county, city, street, number;
			istringstream iss(addressData);
			getline(iss, country, ',');
			getline(iss, county, ',');
			getline(iss, city, ',');
			getline(iss, street, ',');
			getline(iss, number, ',');
			user = PublicUser(res->getString("lastName"),
				res->getString("firstName"),
				res->getString("email"),
				res->getString("password"),
				Address(country, county, city, street, number));
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

void PublicUserRepository::insertIntoDatabase(PublicUser& user) {
	sql::Connection* con = dbConnector.establishConnection();
	Address adr = user.getAddress();
	string addressString = adr.getCountry() + ", " + adr.getCounty() + ", " + adr.getCity() + ", " + adr.getStreet() + ", " + adr.getNumber();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO users (user_type, email, password, lastName, firstName, address) VALUES('public', ?, ?, ?, ?, ?)");
		pstmt->setString(1, user.getEmail());
		pstmt->setString(2, user.getPassword());
		pstmt->setString(3, user.getLastName());
		pstmt->setString(4, user.getFirstName());
		pstmt->setString(5, addressString);
		pstmt->executeUpdate();
		delete pstmt;
		cout << "User added successfuly into database." << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not insert the public user into database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}
