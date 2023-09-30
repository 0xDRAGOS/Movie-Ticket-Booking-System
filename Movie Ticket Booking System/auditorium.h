#pragma once
#include "database_connection.h"
#include "theatre.h"
#define MAX_AUDITORIUM_ROWS 5
#define MAX_AUDITORIUM_COLUMNS 5
#define ADULT_PRICE_2D_BEFORE_17_MT 25
#define ADULT_PRICE_2D_BEFORE_17_FS 28
#define ADULT_PRICE_2D_AFTER_17_MT 26
#define ADULT_PRICE_2D_AFTER_17_FS 31
#define STUDENT_PRICE_2D_MT 22
#define STUDENT_PRICE_2D_FS 24
#define CHILD_PRICE_2D_MT 18
#define CHILD_PRICE_2D_FS 20
#define FAMILY_2D_1_ADULT_MT 36
#define FAMILY_2D_1_ADULT_FS 40
#define FAMILY_2D_2_ADULTS_MT 54
#define FAMILY_2D_2_ADULTS_FS 60
#define FAMILY_2D_3_ADULTS_MT 72
#define FAMILY_2D_3_ADULTS_FS 80
#define GROUP_2D_4_STUDENTS_MT 78
#define GROUP_2D_4_STUDENTS_FS 78
#define PREVIEW_2D 35
#define ADULT_PRICE_3D_BEFORE_17_MT 28
#define ADULT_PRICE_3D_BEFORE_17_FS 30
#define ADULT_PRICE_3D_AFTER_17_MT 29
#define ADULT_PRICE_3D_AFTER_17_FS 33
#define STUDENT_PRICE_3D_MT 24
#define STUDENT_PRICE_3D_FS 27
#define CHILD_PRICE_3D_MT 20
#define CHILD_PRICE_3D_FS 22
#define FAMILY_3D_1_ADULT_MT 40
#define FAMILY_3D_1_ADULT_FS 44
#define FAMILY_3D_2_ADULTS_MT 60
#define FAMILY_3D_2_ADULTS_FS 66
#define FAMILY_3D_3_ADULTS_MT 80
#define FAMILY_3D_3_ADULTS_FS 88
#define GROUP_3D_4_STUDENTS_MT 84
#define GROUP_3D_4_STUDENTS_FS 84
#define PREVIEW_3D 35

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
};