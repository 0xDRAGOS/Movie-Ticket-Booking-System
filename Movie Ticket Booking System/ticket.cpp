#include "ticket.h"

Ticket::Ticket(const double& price) 
	: price(price) {}

double Ticket::getPrice() { return price; };

void Ticket::setPrice(double& newPrice) { this->price = newPrice; };

Ticket TicketInterface::displayCalculateTicketPriceBasedOnOptions() {
	Ticket ticket;
	Date date;
	double price;
	int option, movie_type;
	cout << " 1 - Adult; 2 - Pupils, students, pensioners; 3 - Child under 12 years old; 4 - Family 2 pers*(1 adult + 1 child); " << endl;
	cout << " 5 - Family 3 pers*(2 adult + 1 child); 6 - Family 4 pers*(3 adult + 1 child); 7 - Group 4 pers*(4 pupils/students); 8 - Preview;" << endl;
	cout << "Enter option: "; cin >> option;
	while (option < 1 && option > 9) {
		cout << "Invalid option, retrying..." << endl;
		cout << "Enter option: "; cin >> option;
	}

	cout << "1 - 2D MOVIE; 2 - 3D MOVIE" << endl;
	cout << "Enter type of movie: "; cin >> movie_type;
	while (movie_type < 1 && movie_type > 2) {
		cout << "Invalid movie_type, retrying..." << endl;
		cout << "Enter movie_type: "; cin >> movie_type;
	}

	switch (option)
	{
	case 1:
		if (movie_type == 1) {
			if (date.getLocalTimeHour() < 17) {
				if (date.getLocalTimeDayOfWeek() > 4) price = ADULT_PRICE_2D_BEFORE_17_MT;
				else price = ADULT_PRICE_2D_AFTER_17_MT;
			}
			else {
				if (date.getLocalTimeDayOfWeek() > 4) price = ADULT_PRICE_3D_BEFORE_17_FS;
				else price = ADULT_PRICE_3D_AFTER_17_FS;
			}
		}
		else {
			if (date.getLocalTimeHour() < 17) {
				if (date.getLocalTimeDayOfWeek() > 4) price = ADULT_PRICE_3D_BEFORE_17_MT;
				else price = ADULT_PRICE_3D_AFTER_17_MT;
			}
			else {
				if (date.getLocalTimeDayOfWeek() > 4) price = ADULT_PRICE_3D_BEFORE_17_FS;
				else price = ADULT_PRICE_3D_AFTER_17_FS;
			}
		}
		break;
	case 2:
		if (movie_type == 1) {
			if (date.getLocalTimeDayOfWeek() > 4) price = STUDENT_PRICE_2D_MT;
			else price = STUDENT_PRICE_2D_FS;
		}
		else {
			if (date.getLocalTimeDayOfWeek() > 4) price = STUDENT_PRICE_3D_MT;
			else price = STUDENT_PRICE_3D_FS;
		}
		break;
	case 3:
		if (movie_type == 1) {
			if (date.getLocalTimeDayOfWeek() > 4) price = CHILD_PRICE_2D_MT;
			else price = CHILD_PRICE_2D_FS;
		}
		else {
			if (date.getLocalTimeDayOfWeek() > 4) price = CHILD_PRICE_3D_MT;
			else price = CHILD_PRICE_3D_FS;
		}
		break;
	case 4:
		if (movie_type == 1) {
			if (date.getLocalTimeDayOfWeek() > 4) price = FAMILY_2D_1_ADULT_MT;
			else price = FAMILY_2D_1_ADULT_FS;
		}
		else {
			if (date.getLocalTimeDayOfWeek() > 4) price = FAMILY_3D_1_ADULT_MT;
			else price = FAMILY_3D_1_ADULT_FS;
		}
		break;
	case 5:
		if (movie_type == 1) {
			if (date.getLocalTimeDayOfWeek() > 4) price = FAMILY_2D_2_ADULTS_MT;
			else price = FAMILY_2D_2_ADULTS_FS;
		}
		else {
			if (date.getLocalTimeDayOfWeek() > 4) price = FAMILY_3D_2_ADULTS_MT;
			else price = FAMILY_3D_2_ADULTS_FS;
		}
		break;
	case 6:
		if (movie_type == 1) {
			if (date.getLocalTimeDayOfWeek() > 4) price = FAMILY_2D_3_ADULTS_MT;
			else price = FAMILY_2D_3_ADULTS_FS;
		}
		else {
			if (date.getLocalTimeDayOfWeek() > 4) price = FAMILY_3D_3_ADULTS_MT;
			else price = FAMILY_3D_3_ADULTS_FS;
		}
		break;
	case 7:
		if (movie_type == 1) {
			if (date.getLocalTimeDayOfWeek() > 4) price = GROUP_2D_4_STUDENTS_MT;
			else price = GROUP_2D_4_STUDENTS_FS;
		}
		else {
			if (date.getLocalTimeDayOfWeek() > 4) price = GROUP_3D_4_STUDENTS_MT;
			else price = GROUP_3D_4_STUDENTS_FS;
		}
		break;
	case 8:
		if (movie_type == 1) price = PREVIEW_2D;
		else price = PREVIEW_3D;
		break;
	default:
		break;
	}
	cout << "Ticket price: " << price << endl;
	ticket.setPrice(price);
	return ticket;
}

int TicketRepository::getTicketID(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Theatre& theatre) {
	sql::Connection* con = dbConnector.establishConnection();
	int id;
	try {
		PublicUserRepository userRep;
		int user_id = userRep.getPublicUserID(user);
		MovieRepository movieRep;
		int movie_id = movieRep.getMovieID(movie, auditorium, theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id FROM tickets WHERE user_id = ? AND movie_id = ? and price = ?;");
		pstmt->setInt(1, user_id);
		pstmt->setInt(2, movie_id);
		pstmt->setDouble(3, ticket.getPrice());
		sql::ResultSet* res = pstmt->executeQuery();
		id = res->getInt("id");
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could get ticket id. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return id;
}

void TicketRepository::insertIntoDatabase(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Theatre& theatre, double& price) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		PublicUserRepository userRep;
		int user_id = userRep.getPublicUserID(user);
		MovieRepository movieRep;
		int movie_id = movieRep.getMovieID(movie, auditorium, theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO tickets (user_id, movie_id, price) VALUES(?, ?, ?);");
		pstmt->setInt(1, user_id);
		pstmt->setInt(2, movie_id);
		pstmt->setDouble(3, ticket.getPrice());
		pstmt->executeUpdate();
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not insert ticket into database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

void TicketRepository::updateTicketPrice(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Theatre& theatre, double& newPrice) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TicketRepository ticketRep;
		int ticket_id = ticketRep.getTicketID(ticket, user, movie, auditorium, theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE tickets SET price = ? WHERE id = ?;");
		pstmt->setDouble(1, newPrice);
		pstmt->setInt(2, ticket_id);
		pstmt->executeUpdate();
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not update the ticket. Error: " << e.what();
		exit(1);
	}
	dbConnector.closeConnection(con);
}