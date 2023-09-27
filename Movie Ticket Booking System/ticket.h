#pragma once
#include "teathre.h"
#include "public_user.h"

class Movie;
class Teathre;
class Auditorium;

class Ticket {
private:
	double price;
public:
	Ticket(const double& price = 0.0);
	double getPrice();
	void setPrice(double& newPrice);
};

class TicketRepository {
private:
	DatabaseConnector dbConnector;
public:
	TicketRepository() {}
	int getTicketID(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Teathre& teathre);
	void insertIntoDatabase(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Teathre& teathre, double& price);
	void updateTicketPrice(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Teathre& teathre, double& newPrice);
};