#pragma once
#include "database_connection.h"
#include "teathre.h"

class Teathre;

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
	int getAuditoriumID(Auditorium& auditorium, Teathre& teathre);
	void updateAuditorium(Auditorium& auditorium, Teathre& teathre, const string& field, const int& value);
	void insertIntoDatabase(Auditorium& auditorium, Teathre& teathre);
};