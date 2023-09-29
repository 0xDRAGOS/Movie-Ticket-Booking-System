#include "theatre.h"

Theatre::Theatre(const string& name, const Address& address)
	: name(name), address(address), auditoriums(nullptr), movies(nullptr), auditoriums_number(0), movies_number(0) {}

Theatre::~Theatre() {
	delete[] auditoriums;
	delete[] movies;
}

const string Theatre::getName() { return name; };
const Address Theatre::getAddress() { return address; };
void Theatre::setName(const string& newName) { this->name = name; };
void Theatre::setAddress(const Address& newAddress) { this->address = newAddress; };

void Theatre::addAuditorium(const Auditorium& auditorium) {
	auditoriums[auditoriums_number++] = auditorium;
}

void Theatre::addMovie(const Movie& movie) {
	movies[movies_number++] = movie;
}

Theatre TheatreInterface::displayTheatres() {
	TheatreRepository theatreRep;
	Theatre theatre;
	Theatre* theatres = nullptr;
	int total_theatres = theatreRep.getNumberOfTheatres();
	int option;
	int count = 0;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* res = stmt->executeQuery("SELECT name, address FROM theatres;");
		while (res->next()) {
			theatre.setName(res->getString("name"));
			string addressData = res->getString("address");
			string country, county, city, street, number;
			istringstream iss(addressData);
			getline(iss, country, ',');
			getline(iss, county, ',');
			getline(iss, city, ',');
			getline(iss, street, ',');
			getline(iss, number, ',');
			theatre.setAddress(Address(country, county, city, street, number));

			if (count == 0) {
				theatres = new Theatre[total_theatres];
			}
			theatres[count] = theatre;
		}
		cout << "Enter option between 1 and " << total_theatres << ":"; cin >> option;
		while (option < 1 || option > total_theatres) {
			cout << "Invalid option, retrying..." << endl;
			cout << "Enter option: "; cin >> option;
		}
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not display theatres. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);

	Theatre selectedTheatre = theatres[option - 1];
	delete[] theatres;
	return selectedTheatre;
}

int TheatreRepository::getTheatreID(Theatre& theatre) {
	sql::Connection* con = dbConnector.establishConnection();
	int id;
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id FROM theatres WHERE name = ?;");
		pstmt->setString(1, theatre.getName());
		sql::ResultSet* res = pstmt->executeQuery();
		id = res->getInt("id");
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get theatre id. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return id;
}

int TheatreRepository::getNumberOfTheatres() {
	int number;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* res = stmt->executeQuery("SELECT COUNT(DISTINCT name) AS unique_theatre_count FROM theatres;");
		if (res->next()) {
			number = res->getInt("unique_theatre_count");
			}
		}
	catch (sql::SQLException& e) {
		cerr << "Could not get number of theatres. Error: " << e.what() << endl;
		exit(1);
}
return number;
}

void TheatreRepository::insertIntoDatabase(Theatre& theatre) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO theatres (name, address) VALUES (?, ?);");
		pstmt->setString(1, theatre.getName());
		pstmt->setString(2, theatre.getAddress().toString());
		pstmt->executeUpdate();
		delete pstmt;
		cout << "Theatre added successfuly into database." << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not insert the theatre into database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

void TheatreRepository::listMovies(Theatre& theatre) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TheatreRepository theatreRep;
		int theatre_id = theatreRep.getTheatreID(theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT *, YEAR(launch_date) AS launch_date_year, MONTH(launch_date) AS launch_date_month, DAY(launch_date) AS launch_date_day FROM movies WHERE theatre_id = ?;");
		pstmt->setInt(1, theatre_id);
		sql::ResultSet* res = pstmt->executeQuery();
		int count = 0;
		while (res->next()) {
			Movie movie(res->getString("name"),
				res->getString("format"),
				res->getString("rating"),
				res->getString("director"),
				res->getString("actors"),
				res->getString("trailer_url"),
				res->getString("genre"),
				res->getString("language"),
				res->getString("producer"),
				res->getString("country"),
				Date(res->getInt("launch_date_year"), res->getInt("launch_date_month"), res->getInt("launch_date_day"))
			);
			MovieInterface movieInt;
			movieInt.displayMovie(movie);
			if (++count % 3 == 0) cout << endl;
		}
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not list the movies. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}