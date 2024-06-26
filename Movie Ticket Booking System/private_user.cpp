#include "private_user.h"

PrivateUser::PrivateUser(const string& lastName, const string& firstName, const string& email, const string& password)
	: User(lastName, firstName, email, password) {}

void PrivateUser::updateCredentials(PublicUser& user) {
	UserRepository userRep;

	try {
		string email, password;
		cout << "Enter new email: "; cin >> email;
		cout << "Enter new password: "; cin >> password;
		userRep.updateUser(user, "email", email);
		userRep.updateUser(user, "password", password);
		cout << "User credentials updated successfully!" << endl;
	}
	catch(const std::exception& e){
		cerr << "An unexpected error occurred: " << e.what() << endl;
	}
}

void PrivateUser::updateTheatre(Theatre& theatre) {
	TheatreRepository theatreRep;

	try {
		string name;
		cout << "Enter new name: "; cin >> name;
		theatreRep.updateName(theatre, name);
		cout << "Theatre name updated successfully!" << endl;
	}
	catch (const std::exception& e) {
		cerr << "An unexpected error occurred: " << e.what() << endl;
	}
}

void PrivateUser::updateMovieDetails(Movie& movie) {
	MovieRepository movieRep;

	try {
		string name, format, rating, director, actors, trailer_url, genre, language, producer, country;
		Date launch_date;
		cout << "Enter new name: "; cin >> name;
		cout << "Enter new format: "; cin >> format;
		cout << "Enter new rating: "; cin >> rating;
		cout << "Enter new director: "; cin >> director;
		cout << "Enter new actors: "; cin >> actors;
		cout << "Enter new trailer url: "; cin >> trailer_url;
		cout << "Enter new genre: "; cin >> genre;
		cout << "Enter new language: "; cin >> language;
		cout << "Enter new producer: "; cin >> producer;
		cout << "Enter new country: "; cin >> country;
		cout << "Enter new launch date: "; cin >> launch_date;
		movieRep.updateMovie(movie, "name", name);
		movieRep.updateMovie(movie, "format", format);
		movieRep.updateMovie(movie, "rating", rating);
		movieRep.updateMovie(movie, "director", director);
		movieRep.updateMovie(movie, "actors", actors);
		movieRep.updateMovie(movie, "trailer_url", trailer_url);
		movieRep.updateMovie(movie, "genre", genre);
		movieRep.updateMovie(movie, "language", language);
		movieRep.updateMovie(movie, "producer", producer);
		movieRep.updateMovie(movie, "country", country);
		movieRep.updateMovie(movie, "launch_date", launch_date);
		cout << "Movie updated successfully!" << endl;
	}
	catch (const std::exception& e) {
		cerr << "An unexpected error occurred: " << e.what() << endl;
	}
}

void PrivateUser::updateAuditorium(Auditorium& auditorium, Theatre& theatre) {
	AuditoriumRepository auditoriumRep;

	try {
		int auditorium_number, seats_number;
		cout << "Enter new auditorium number: "; cin >> auditorium_number;
		cout << "Enter new seats_number: "; cin >> seats_number;
		auditoriumRep.updateAuditorium(auditorium, theatre, "auditorium_number", auditorium_number);
		auditoriumRep.updateAuditorium(auditorium, theatre, "seats_number", seats_number);
		cout << "Auditorium updated successfully!" << endl;
	}
	catch (const std::exception& e) {
		cerr << "An unexpected error occurred: " << e.what() << endl;
	}
}

void PrivateUser::addTheatre(Theatre& theatre) {
	TheatreRepository theatreRep;
	theatreRep.insertIntoDatabase(theatre);
}

void PrivateUser::addAuditorium(Auditorium& auditorium, Theatre& theatre) {
	AuditoriumRepository auditoriumRep;
	auditoriumRep.insertIntoDatabase(auditorium, theatre);
}

void PrivateUser::addMovie(Movie& movie, Auditorium& auditorium, Theatre& theatre) {
	MovieRepository movieRep;
	movieRep.insertIntoDatabase(movie, theatre, auditorium);
}

void PrivateUser::deleteTheatre(Theatre& theatre) {
	TheatreRepository theatreRep;
	theatreRep.deleteFromDatabase(theatre);
}

void PrivateUser::deleteAuditorium(Auditorium& auditorium, Theatre& theatre) {
	AuditoriumRepository auditoriumRep;
	auditoriumRep.deleteFromDatabase(auditorium, theatre);
}

void PrivateUser::deleteMovie(Movie& movie, Auditorium& auditorium, Theatre& theatre) {
	MovieRepository movieRep;
	movieRep.deleteFromDatabase(movie, theatre, auditorium);
}

int PrivateUserInterface::displayMainMenu() {
	int option;

	cout << "-----------------------------------------------------------------------------------" << endl;
	cout << "| 1 - Change user email and password                                              |" << endl;
	cout << "| 2 - Modify theatre details      | 3 - Add theatre      | 4 - Delete theatre     |" << endl;
	cout << "| 5 - Modify auditorium details   | 6 - Add auditorium   | 7 - Delete auditorium  |" << endl;
	cout << "| 8 - Modify movie details        | 9 - Add movie        | 10 - Delete movie      |" << endl;
	cout << "| 11 - Exit                                                                       |" << endl;
	cout << "-----------------------------------------------------------------------------------" << endl;

	cout << "Enter option: "; cin >> option;
	while (option < 1 || option > 11) {
		cout << "Invalid option, retrying..." << endl;
		cout << "Enter option: "; cin >> option;
	}
	return option;
}

PrivateUser PrivateUserRepository::loadPrivateUser(const string& inputEmail, const string& inputPassword) {
	PrivateUser user;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM users WHERE user_type = 'private' AND email = ? AND password = ?;");
		pstmt->setString(1, inputEmail);
		pstmt->setString(2, inputPassword);
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()) {
			user = PrivateUser(res->getString("lastName"),
				res->getString("firstName"),
				res->getString("email"),
				res->getString("password")
			);
		}
		cout << "User loaded up successfully." << endl;
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not load user. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return user;
}