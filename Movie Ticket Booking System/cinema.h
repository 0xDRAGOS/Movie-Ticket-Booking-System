#pragma once

#include <string>
#include "database_connection.h"
#include "date.h"
using std::string;

class Audithorium {
private:
	bool** seats;
	int rows;
	int columns;
	int audithorium_number;
public:
	Audithorium(const int& audithoirum_number, const int& rows, const int& columns);
	~Audithorium();
	void changeSeatStatus(bool status, int row, int column);
	const int getAudithoriumNumber() { return audithorium_number; };
	void setAudithoriumNumber(int newAudithoriumNumber) { this->audithorium_number = newAudithoriumNumber; };
};

Audithorium::Audithorium(const int& audithoirum_number, const int& rows, const int& columns): audithorium_number(audithoirum_number), rows(rows), columns(columns) {
	seats = new bool*[rows];
	for (int i = 0; i < rows; i++) {
		seats[i] = new bool[columns];
		for (int j = 0; j < columns; j++)
			seats[i][j] = true;
	}
}

Audithorium::~Audithorium() {
	for (int i = 0; i < rows; i++)
		delete[] seats[i];
	delete[] seats;
}

void Audithorium::changeSeatStatus(bool status, int row, int column) {
	if (row >= 0 && row < rows && column >= 0 && column < columns) {
		seats[row][column] = status;
	}
	else {
		throw "Invalid row/column";
	}
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
	void insertIntoDatabase(Movie& movie);
	//Movie loadMovie();
	template <typename T>
	void updateMovie(Movie& movie, const string& field, const T& value);
};

void MovieRepository::insertIntoDatabase(Movie& movie) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO movies () VALUES()");
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
		const string query = "UPDATE movies SET " + field + " = ? WHERE email = ? AND password = ?";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setString(1, value);
		pstmt->setString(2, movie.getEmail());
		pstmt->setString(3, movie.getPassword());
		int rowsUpdated = pstmt->executeUpdate();

		if (rowsUpdated > 0) {
			cout << "Movie" << movie.getFirstName() << " " << movie.getLastName() << " modified with success." << endl;
			con->commit();
		}
		else {
			cout << "Movie" << movie.getFirstName() << " " << movie.getLastName() << " not found." << endl;
		}
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not update movie ( " << movie.getFirstName() << " " << movie.getLastName() << "). Error message : " << e.what() << endl;
		con->rollback();
		exit(1);
	}
	dbConnector.closeConnection(con);
}