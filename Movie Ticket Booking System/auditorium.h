#pragma once
#include "database_connection.h"
#include "theatre.h"
#include "constants.h"

class Theatre;
class Movie;

class Auditorium {
private:
	bool seats[MAX_AUDITORIUM_ROWS][MAX_AUDITORIUM_COLUMNS];
	int rows;
	int columns;
	int seats_number;
	int auditorium_number;
public:
	Auditorium(const int& auditorium_number = 0, const int& seats_number = 0, const int& rows = 0, const int& columns = 0);
	virtual ~Auditorium();
	const int getAuditoriumNumber();
	const int getSeatNumber();
	const int getRows();
	const int getColumns();
	const bool getSeatValue(int row, int column);
	void setAuditoriumNumber(int newAuditoriumNumber);
	void setSeatsNumber(int newSeatsNumber);
	void setRows(int newRows);
	void setColumns(int newColumns);
	void changeSeatStatus(bool status, int row, int column);
};

class AuditoriumInterface {
private:
	DatabaseConnector dbConnector;
public:
	void displayAuditoriumSeats(Auditorium& auditorium);
	void displayAuditorium(Auditorium& auditorium);
	Auditorium displayAuditoriums(Movie& movie, Theatre& theatre);
	void displaySelectSeat(Auditorium& auditorium);
	Auditorium readAuditorium();
};

class AuditoriumRepository {
private:
	DatabaseConnector dbConnector;
public:
	AuditoriumRepository() {}
	int getAuditoriumID(Auditorium& auditorium, Theatre& theatre);
	int getNumberOfAuditoriumsByTheatre(Theatre& theatre);
	void updateAuditorium(Auditorium& auditorium, Theatre& theatre, const string& field, const int& value);
	void insertIntoDatabase(Auditorium& auditorium, Theatre& theatre);
	void deleteFromDatabase(Auditorium& auditorium, Theatre& theatre);
};