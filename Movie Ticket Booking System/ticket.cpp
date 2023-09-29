#include "ticket.h"

Ticket::Ticket(const double& price) 
	: price(price) {}

double Ticket::getPrice() { return price; };

void Ticket::setPrice(double& newPrice) { this->price = newPrice; };

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