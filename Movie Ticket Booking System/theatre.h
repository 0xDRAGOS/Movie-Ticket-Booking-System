#pragma once
#include <sstream>
#include <string>
#include "movie.h"
#include "auditorium.h"
#include "database_connection.h"
#include "address.h"
using std::string;
using std::cerr;
using std::endl;
using std::to_string;
using std::getline;
using std::istringstream;

class Auditorium;
class Movie;

class Theatre {
private:
	Auditorium *auditoriums;
	Movie* movies;
	int auditoriums_number;
	int movies_number;
	string name;
	Address address;
public:
	Theatre(const string& name = "", const Address& address = Address());
	virtual ~Theatre();
	const string getName();
	const Address getAddress();
	void setName(const string& newName);
	void setAddress(const Address& newAddress);
	void addAuditorium(const Auditorium& auditorium);
	void addMovie(const Movie& movie);
};

class TheatreInterface {
private:
	DatabaseConnector dbConnector;
public:
	void displayTheatre(Theatre& theatre);
	Theatre displayTheatres();
};

class TheatreRepository {
private:
	DatabaseConnector dbConnector;
public:
	TheatreRepository() {}
	int getTheatreID(Theatre& theatre);
	int getNumberOfTheatres();
	void insertIntoDatabase(Theatre& theatre);
	void listMovies(Theatre& theatre);
};