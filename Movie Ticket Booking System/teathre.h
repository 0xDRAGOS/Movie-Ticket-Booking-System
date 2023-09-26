#pragma once

#include <string>
#include "movie.h"
#include "database_connection.h"
#include "date.h"
#include "address.h"
using std::string;
using std::cerr;
using std::endl;
using std::to_string;

class Teathre {
private:
	Auditorium* auditoriums;
	Movie* movies;
	int auditoriums_number;
	int movies_number;
	string name;
	Address address;
public:
	Teathre(const string& name, const Address& address);
	virtual ~Teathre();
	const string getName();
	const Address getAddress();
	void setName(const string& newName);
	void setAddress(const Address& newAddress);
	void addAuditorium(const Auditorium& auditorium);
	void addMovie(const Movie& movie);
};

class TeathreRepository {
private:
	DatabaseConnector dbConnector;
public:
	TeathreRepository() {}
	int getTeathreID(Teathre& teathre);
	void insertIntoDatabase(Teathre& teathre);
	void listMovies(Teathre& teathre);
};