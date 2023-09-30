#pragma once
#include "theatre.h"
#include "public_user.h"

class Movie;
class Theatre;
class Auditorium;

class Ticket {
private:
	double price;
public:
	Ticket(const double& price = 0.0);
	double getPrice();
	void setPrice(double& newPrice);
};

class TicketInterface {
public:
	TicketInterface() {}
	Ticket displayCalculateTicketPriceBasedOnOptions();
};

class TicketRepository {
private:
	DatabaseConnector dbConnector;
public:
	TicketRepository() {}
	int getTicketID(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Theatre& theatre);
	void insertIntoDatabase(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Theatre& theatre, double& price);
	void updateTicketPrice(Ticket& ticket, PublicUser& user, Movie& movie, Auditorium& auditorium, Theatre& theatre, double& newPrice);
};