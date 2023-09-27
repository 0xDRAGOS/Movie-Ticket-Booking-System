#include "auditorium.h"

Auditorium::Auditorium(const int& audithoirum_number, const int& seats_number, const int& rows, const int& columns)
	: auditorium_number(auditorium_number), seats_number(seats_number), rows(rows), columns(columns) {
	seats = new bool* [rows];
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

const int Auditorium::getAuditoriumNumber() { return auditorium_number; };
const int Auditorium::getSeatNumber() { return seats_number; };

void Auditorium::setAuditoriumNumber(int newAuditoriumNumber) { this->auditorium_number = newAuditoriumNumber; };
void Auditorium::setSeatsNumber(int newSeatsNumber) { this->seats_number = newSeatsNumber; };

void Auditorium::changeSeatStatus(bool status, int row, int column) {
	if (row >= 0 && row < rows && column >= 0 && column < columns) {
		seats[row][column] = status;
	}
	else {
		throw "Invalid row/column";
	}
}

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

void AuditoriumRepository::insertIntoDatabase(Auditorium& auditorium, Teathre& teathre) {
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