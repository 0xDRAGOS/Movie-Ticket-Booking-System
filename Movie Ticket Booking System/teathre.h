#pragma once

#include <string>
#include "database_connection.h"
#include "date.h"
#include "address.h"
using std::string;
using std::cerr;
using std::endl;

class Teathre;

class Auditorium {
private:
	bool** seats;
	int rows;
	int columns;
	int seats_number;
	int auditorium_number;
public:
	Auditorium(const int& audithoirum_number, const int& seats_number, const int& rows, const int& columns);
	~Auditorium();
	void changeSeatStatus(bool status, int row, int column);
	const int getAuditoriumNumber() { return auditorium_number; };
	const int getSeatNumber() { return seats_number; };
	void setAuditoriumNumber(int newAuditoriumNumber) { this->auditorium_number = newAuditoriumNumber; };
	void setSeatsNumber(int newSeatsNumber) { this->seats_number = newSeatsNumber; };
};

Auditorium::Auditorium(const int& audithoirum_number, const int& seats_number, const int& rows, const int& columns)
	: auditorium_number(auditorium_number), seats_number(seats_number), rows(rows), columns(columns) {
	seats = new bool*[rows];
	for (int i = 0; i < rows; i++) {
		seats[i] = new bool[columns];
		for (int j = 0; j < columns; j++)
			seats[i][j] = true;
	}
}

Auditorium::~Auditorium() {
	for (int i = 0; i < rows; i++)
		delete[] seats[i];
	delete[] seats;
}

void Auditorium::changeSeatStatus(bool status, int row, int column) {
	if (row >= 0 && row < rows && column >= 0 && column < columns) {
		seats[row][column] = status;
	}
	else {
		throw "Invalid row/column";
	}
}

class AuditoriumRepository {
private:
	DatabaseConnector dbConnector;
public:
	AuditoriumRepository() {}
	int getAuditoriumID(Auditorium& auditorium, Teathre& teathre);
	void updateAuditorium(Auditorium& auditorium, Teathre& teathre, const string& field, const int& value);
	void insertIntoDatabase(Auditorium& auditorium, Teathre& teathre);
};

int AuditoriumRepository::getAuditoriumID(Auditorium& auditorium, Teathre& teathre) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		int id, teathre_id;
		TeathreRepository teathreRep;
		teathre_id = teathreRep.getTeathreID(teathre);
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id FROM auditoriums WHERE auditorium_number = ? AND teathre_id = ?;");
		pstmt->setInt(1, auditorium.getAuditoriumNumber());
		pstmt->setInt(2, teathre_id);
		sql::ResultSet* res = pstmt->executeQuery();
		id = res->getInt("id");
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get auditorium id. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

void AuditoriumRepository::updateAuditorium(Auditorium& auditorium, Teathre& teathre, const string& field, const int& value) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TeathreRepository teathreRep;
		int teathre_id = teathreRep.getTeathreID(teathre);
		const string query = "UPDATE auditoriums SET " + field + " = ? WHERE auditorium_number = ? AND teathre_id = ?;";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setInt(1, value);
		pstmt->setInt(2, auditorium.getAuditoriumNumber());
		pstmt->setInt(3, teathre_id);
		int rowsUpdated = pstmt->executeUpdate();

		if (rowsUpdated > 0) {
			cout << "Auditorium" << auditorium.getAuditoriumNumber() << " modified with success." << endl;
			con->commit();
		}
		else {
			cout << "Auditorium" << auditorium.getAuditoriumNumber() << " not found." << endl;
		}
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not update auditorium ( " << auditorium.getAuditoriumNumber() << "). Error message : " << e.what() << endl;
		con->rollback();
		exit(1);
	}
	dbConnector.closeConnection(con);
}

void AuditoriumRepository::insertIntoDatabase(Auditorium& auditorium, Teathre& teathre){
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TeathreRepository teathreRep;
		int teathre_id = teathreRep.getTeathreID(teathre);
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO auditoriums (teathre_id, auditorium_number, seats_number) VALUES (?, ?, ?);");
		pstmt->setInt(1, teathre_id);
		pstmt->setInt(2, auditorium.getAuditoriumNumber());
		pstmt->setInt(3, auditorium.getSeatNumber());
		pstmt->executeUpdate();
		delete pstmt;
		cout << "Auditorium added successfuly into database." << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not insert the auditorium into database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

class Movie {
private:
	string name;
	string format;
	string rating;
	string director;
	string actors;
	string trailer_url;
	string genre;
	string language;
	string producer;
	string country;
	Date launch_date;
public:
	Movie(const string& name, const string& format, const string& rating, const string& director, const string& actors, const string& trailer_url, const string& genre, const string& language, const string& producer, const string& country, const Date& launch_date)
		: name(name), format(format), rating(rating), director(director), actors(actors), trailer_url(trailer_url), genre(genre), language(language), producer(producer), country(country), launch_date(launch_date) {}
    const string getName(){ return name;}
    const string getFormat(){ return format; }
    const string getRating(){ return rating; }
	const string getDirector(){ return director; }
	const string getActors(){ return actors; }
	const string getTrailerURL(){ return trailer_url; }
	const string getGenre(){ return genre; }
	const string getLanguage(){ return language; }
	const string getProducer(){ return producer; }
	const string getCountry(){ return country; }
	const Date getLaunchDate(){ return launch_date; }
	void setFormat(const string& newFormat) { format = newFormat; }
	void setRating(const string& newRating) { rating = newRating; }
	void setDirector(const string& newDirector) { director = newDirector; }
	void setActors(const string& newActors) { actors = newActors; }
	void setTrailerURL(const string& newTrailerURL) { trailer_url = newTrailerURL; }
	void setGenre(const string& newGenre) { genre = newGenre; }
	void setLanguage(const string& newLanguage) { language = newLanguage; }
	void setProducer(const string& newProducer) { producer = newProducer; }
	void setCountry(const string& newCountry) { country = newCountry; }
	void setLaunchDate(const Date& newLaunchDate) { launch_date = newLaunchDate; }
};

class MovieRepository {
private:
	DatabaseConnector dbConnector;
public:
	MovieRepository() {}
	void insertIntoDatabase(Movie& movie, Teathre& teathre, Auditorium& auditorium);
	//Movie loadMovie();
	template <typename T>
	void updateMovie(Movie& movie, const string& field, const T& value);
};

void MovieRepository::insertIntoDatabase(Movie& movie, Teathre& teathre, Auditorium& auditorium) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TeathreRepository teathreRep;
		int teathre_id = teathreRep.getTeathreID(teathre);
		AuditoriumRepository auditoriumRep;
		int auditorium_id = auditoriumRep.getAuditoriumID(auditorium, teathre);
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO movies (teathre_id, auditorium_id, name, format, rating, director, actors, trailer_url, genre, language, producer, country, launch_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
		Date launchDate = movie.getLaunchDate();
		string launchDateString = to_string(launchDate.getYear()) + "-" + to_string(launchDate.getMonth()) + "-" + to_string(launchDate.getDay());
		pstmt->setInt(1, teathre_id);
		pstmt->setInt(2, auditorium_id);
		pstmt->setString(3, movie.getName());
		pstmt->setString(4, movie.getFormat());
		pstmt->setString(5, movie.getRating());
		pstmt->setString(6, movie.getDirector());
		pstmt->setString(7, movie.getActors());
		pstmt->setString(8, movie.getTrailerURL());
		pstmt->setString(9, movie.getGenre());
		pstmt->setString(10, movie.getLanguage());
		pstmt->setString(11, movie.getProducer());
		pstmt->setString(12, movie.getCountry());
		pstmt->setString(13, launchDateString);

		pstmt->executeUpdate();
		delete pstmt;
		cout << "Movie added successfuly into database." << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not insert the movie into database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

template <typename T>
void MovieRepository::updateMovie(Movie& movie, const string& field, const T& value) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		const string query = "UPDATE movies SET " + field + " = ? WHERE name = ?";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setString(1, value);
		pstmt->setString(2, movie.getName());
		int rowsUpdated = pstmt->executeUpdate();

		if (rowsUpdated > 0) {
			cout << "Movie" << movie.getName() << " modified with success." << endl;
			con->commit();
		}
		else {
			cout << "Movie" << movie.getName() << " not found." << endl;
		}
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not update movie ( " << movie.getName() << "). Error message : " << e.what() << endl;
		con->rollback();
		exit(1);
	}
	dbConnector.closeConnection(con);
}

class Teathre {
private:
	string name;
	Address address;
public:
	Teathre(const string& name, const Address& address) : name(name), address(address) {}
	const string getName() { return name; };
	const Address getAddress() { return address; };
	void setName(const string& newName) { this->name = name; };
	void setAddress(const Address& newAddress) { this->address = newAddress; };
};

class TeathreRepository {
private:
	DatabaseConnector dbConnector;
public:
	TeathreRepository() {}
	int getTeathreID(Teathre& teathre);
};

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