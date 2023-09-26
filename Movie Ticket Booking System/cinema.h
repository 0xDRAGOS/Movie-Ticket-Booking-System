#pragma once

#include <string>

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
