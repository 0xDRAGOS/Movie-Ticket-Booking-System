#pragma once
#include "database_connection.h"
#include "theatre.h"

class Theatre;

class Auditorium {
private:
	bool** seats;
	int rows;
	int columns;
	int seats_number;
	int auditorium_number;
public:
	Auditorium(const int& audithoirum_number = 0, const int& seats_number = 0, const int& rows = 0, const int& columns = 0);
	virtual ~Auditorium();
	const int getAuditoriumNumber();
	const int getSeatNumber();
	void setAuditoriumNumber(int newAuditoriumNumber);
	void setSeatsNumber(int newSeatsNumber);
	void changeSeatStatus(bool status, int row, int column);
};

class AuditoriumRepository {
private:
	DatabaseConnector dbConnector;
public:
	AuditoriumRepository() {}
	int getAuditoriumID(Auditorium& auditorium, Theatre& theatre);
	void updateAuditorium(Auditorium& auditorium, Theatre& theatre, const string& field, const int& value);
	void insertIntoDatabase(Auditorium& auditorium, Theatre& theatre);
};