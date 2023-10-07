#pragma once
#include "public_user.h"
#include "theatre.h"
#include "movie.h"

class PrivateUser : public User {
public:
	PrivateUser(const string& lastName = "", const string& firstName = "", const string& email = "", const string& password = "");
	void updateCredentials(PublicUser& user);
	void updateTheatre(Theatre& theatre);
	void updateAuditorium(Auditorium& auditorium, Theatre& theatre);
	void updateMovieDetails(Movie& movie);
	void addTheatre(Theatre& theatre);
	void addAuditorium(Auditorium& auditorium, Theatre& theatre);
	void addMovie(Movie& movie, Auditorium& auditorium, Theatre& theatre);
	void deleteTheatre(Theatre& theatre);
	void deleteAuditorium(Auditorium& auditorium, Theatre& theatre);
	void deleteMovie(Movie& movie, Auditorium& auditorium, Theatre& theatre);
};

class PrivateUserInterface {
public:
	PrivateUserInterface() {}
	int displayMainMenu();
};

class PrivateUserRepository {
private:
	DatabaseConnector dbConnector;
public:
	PrivateUserRepository() {}
	PrivateUser loadPrivateUser(const string& inputEmail, const string& inputPassword);
};