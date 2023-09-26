#include "teathre.h"

Teathre::Teathre(const string& name, const Address& address)
	: name(name), address(address), auditoriums(nullptr), movies(nullptr), auditoriums_number(0), movies_number(0) {}

Teathre::~Teathre() {
	delete[] auditoriums;
	delete[] movies;
}

const string Teathre::getName() { return name; };
const Address Teathre::getAddress() { return address; };
void Teathre::setName(const string& newName) { this->name = name; };
void Teathre::setAddress(const Address& newAddress) { this->address = newAddress; };

void Teathre::addAuditorium(const Auditorium& auditorium) {
	auditoriums[auditoriums_number++] = auditorium;
}

void Teathre::addMovie(const Movie& movie) {
	movies[movies_number++] = movie;
}

int TeathreRepository::getTeathreID(Teathre& teathre) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		int id;
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id FROM teathres WHERE name = ?;");
		pstmt->setString(1, teathre.getName());
		sql::ResultSet* res = pstmt->executeQuery();
		id = res->getInt("id");
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get teathre id. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

void TeathreRepository::insertIntoDatabase(Teathre& teathre) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO teathres (name, address) VALUES (?, ?);");
		pstmt->setString(1, teathre.getName());
		pstmt->setString(2, teathre.getAddress().toString());
		pstmt->executeUpdate();
		delete pstmt;
		cout << "Teathre added successfuly into database." << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not insert the teathre into database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

void TeathreRepository::listMovies(Teathre& teathre) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TeathreRepository teathreRep;
		int teathre_id = teathreRep.getTeathreID(teathre);
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT *, YEAR(launch_date) AS launch_date_year, MONTH(launch_date) AS launch_date_month, DAY(launch_date) AS launch_date_day FROM movies WHERE teathre_id = ?;");
		pstmt->setInt(1, teathre_id);
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