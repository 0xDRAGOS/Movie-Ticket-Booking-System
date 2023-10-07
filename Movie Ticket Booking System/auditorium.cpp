#include "auditorium.h"

Auditorium::Auditorium(const int& auditorium_number, const int& seats_number, const int& rows, const int& columns)
	: auditorium_number(auditorium_number), seats_number(seats_number), rows(rows), columns(columns) {
	for (int i = 0; i < MAX_AUDITORIUM_ROWS; i++) {
		for (int j = 0; j < MAX_AUDITORIUM_COLUMNS; j++)
			seats[i][j] = true;
	}
}

Auditorium::~Auditorium() {}

const int Auditorium::getAuditoriumNumber() { return auditorium_number; }
const int Auditorium::getSeatNumber() { return seats_number; }
const int Auditorium::getRows() { return rows; }
const int Auditorium::getColumns() { return columns; }
const bool Auditorium::getSeatValue(int row, int column) { return seats[row][column]; }

void Auditorium::setAuditoriumNumber(int newAuditoriumNumber) { this->auditorium_number = newAuditoriumNumber; }
void Auditorium::setSeatsNumber(int newSeatsNumber) { this->seats_number = newSeatsNumber; }
void Auditorium::setRows(int newRows) { this->rows = newRows;}
void Auditorium::setColumns(int newColumns) { this->columns = newColumns; }

void Auditorium::changeSeatStatus(bool status, int row, int column) {
	if (row >= 0 && row < rows && column >= 0 && column < columns) {
		seats[row][column] = status;
	}
	else {
		throw "Invalid row/column";
	}
}

void AuditoriumInterface::displayAuditorium(Auditorium& auditorium) {
	cout << "Auditorium Number: " << auditorium.getAuditoriumNumber();
}

void AuditoriumInterface::displayAuditoriumSeats(Auditorium& auditorium) {
	int rowCount = 0;
	int columnCount = 0;
	cout << "       ";
	for (int i = 0; i < auditorium.getColumns(); i++)
		cout << "C" << ++columnCount << "     ";
	cout << endl;
	for (int i = 0; i < auditorium.getRows(); i++) {
		cout << "R" << ++rowCount << " - ";
		for (int j = 0; j < auditorium.getColumns(); j++) {
			if (auditorium.getSeatValue(i, j) == true) cout << "VACANT ";
			else cout << "OCCUPIED ";
		}
		cout << endl;
	}
}

void AuditoriumInterface::displaySelectSeat(Auditorium& auditorium) {
	int row, column;
	bool seatStatus = false;
	
	while (!seatStatus) {
		do {
			cout << "Insert row between 1 and " << auditorium.getRows() << ": ";
			cin >> row;

			if (row < 1 || row > auditorium.getRows()) {
				cout << "Invalid row, retrying..." << endl;
			}
		} while (row < 1 || row > auditorium.getRows());

		do {
			cout << "Insert column between 1 and " << auditorium.getColumns() << ": ";
			cin >> column;

			if (column < 1 || column > auditorium.getColumns()) {
				cout << "Invalid column, retrying..." << endl;
			}
		} while (column < 1 || column > auditorium.getColumns());

		seatStatus = auditorium.getSeatValue(row, column);
		if (!seatStatus) {
			cout << "This seat is already occupied." << endl;
		}
	}

	if (seatStatus == true) {
		auditorium.changeSeatStatus(false, row - 1, column - 1);
		cout << "Seat at row " << row << ", column " << column << " has been reserved." << endl;
	}
}

Auditorium AuditoriumInterface::displayAuditoriums(Movie& movie, Theatre& theatre){
	TheatreRepository theatreRep;
	Auditorium auditorium;
	Auditorium* auditoriums = nullptr;
	AuditoriumRepository auditoriumRep;
	AuditoriumInterface auditoriumInt;
	int number = auditoriumRep.getNumberOfAuditoriumsByTheatre(theatre);
	auditoriums = new Auditorium[number];
	int count = 0;
	int option;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT auditorium_number, seats_number FROM movie_broadcast JOIN movies ON movies.id = movie_broadcast.movie_id JOIN auditoriums ON auditoriums.id = movies.auditorium_id WHERE movies.theatre_id = ? AND movies.name = ? AND broadcast_date > CURRENT_TIMESTAMP;");
		pstmt->setInt(1, theatreRep.getTheatreID(theatre));
		pstmt->setString(2, movie.getName());
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()) {
			auditorium.setAuditoriumNumber(res->getInt("auditorium_number"));
			auditorium.setSeatsNumber(res->getInt("seats_number"));
			auditorium.setRows(MAX_AUDITORIUM_ROWS);
			auditorium.setColumns(MAX_AUDITORIUM_COLUMNS);
			if (auditoriums != nullptr) {
				auditoriums[count] = auditorium;
			}
			else {
				throw "Could not add the auditorium to auditoriums vector; \n";
			}
			auditoriumInt.displayAuditorium(auditorium);
			cout << " - Option: " << ++count << endl;
		}
		cout << "Enter option between 1 and " << number << ": "; cin >> option;
		while (option < 1 || option > number) {
			cout << "Invalid option, retrying..." << endl;
			cout << "Enter option: "; cin >> option;
		}
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not display auditoriums by movie and theatre. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);

	if (auditoriums != nullptr) {
		Auditorium selectedAuditorium = auditoriums[option - 1];
		delete[] auditoriums;
		return selectedAuditorium;
	}
	else {
		throw "Auditoriums vector is null. Could not return selected auditorium; \n";
	}
}

Auditorium AuditoriumInterface::readAuditorium() {
	Auditorium auditorium;
	int auditorium_number, seats_number;

	cout << "Enter auditorium number: "; cin >> auditorium_number;
	auditorium.setAuditoriumNumber(auditorium_number);
	cout << "Enter seats number: "; cin >> seats_number;
	auditorium.setSeatsNumber(seats_number);

	return auditorium;
}

int AuditoriumRepository::getAuditoriumID(Auditorium& auditorium, Theatre& theatre) {
	sql::Connection* con = dbConnector.establishConnection();
	int id;
	try {
		TheatreRepository theatreRep;
		int theatre_id = theatreRep.getTheatreID(theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id FROM auditoriums WHERE auditorium_number = ? AND theatre_id = ?;");
		pstmt->setInt(1, auditorium.getAuditoriumNumber());
		pstmt->setInt(2, theatre_id);
		sql::ResultSet* res = pstmt->executeQuery();
		if (res->next()) {
			id = res->getInt("id");
		}
		else {
			cerr << "No data found for auditorium: " << auditorium.getAuditoriumNumber() << endl;
		}
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get auditorium id. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return id;
}

int AuditoriumRepository::getNumberOfAuditoriumsByTheatre(Theatre& theatre) {
	int number;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		int id, theatre_id;
		TheatreRepository theatreRep;
		theatre_id = theatreRep.getTheatreID(theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT COUNT(id) as total FROM auditoriums WHERE theatre_id = ?;");
		pstmt->setInt(1, theatre_id);
		sql::ResultSet* res = pstmt->executeQuery();
		if (res->next()) {
			number = res->getInt("total");
		}
		else {
			cout << "Could not find data. " << endl;
		}
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get number of auditoriums by theatre. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return number;
}

void AuditoriumRepository::updateAuditorium(Auditorium& auditorium, Theatre& theatre, const string& field, const int& value) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TheatreRepository theatreRep;
		int theatre_id = theatreRep.getTheatreID(theatre);
		const string query = "UPDATE auditoriums SET " + field + " = ? WHERE auditorium_number = ? AND theatre_id = ?;";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setInt(1, value);
		pstmt->setInt(2, auditorium.getAuditoriumNumber());
		pstmt->setInt(3, theatre_id);
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

void AuditoriumRepository::insertIntoDatabase(Auditorium& auditorium, Theatre& theatre) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TheatreRepository theatreRep;
		int theatre_id = theatreRep.getTheatreID(theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO auditoriums (theatre_id, auditorium_number, seats_number) VALUES (?, ?, ?);");
		pstmt->setInt(1, theatre_id);
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

void AuditoriumRepository::deleteFromDatabase(Auditorium& auditorium, Theatre& theatre) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TheatreRepository theatreRep;
		int theatre_id = theatreRep.getTheatreID(theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM auditoriums WHERE theatre_id = ? AND auditorium_number = ? AND seats_number = ?;");
		pstmt->setInt(1, theatre_id);
		pstmt->setInt(2, auditorium.getAuditoriumNumber());
		pstmt->setInt(3, auditorium.getSeatNumber());
		pstmt->executeUpdate();
		delete pstmt;
		cout << "Auditorium deleted successfully from database." << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not delete the auditorium from database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}