#include "movie.h"

Movie::Movie(const string& name, const string& format, const string& rating, const string& director, const string& actors, const string& trailer_url, const string& genre, const string& language, const string& producer, const string& country, const Date& launch_date)
	: name(name), format(format), rating(rating), director(director), actors(actors), trailer_url(trailer_url), genre(genre), language(language), producer(producer), country(country), launch_date(launch_date) {}

const string Movie::getName() { return name; }
const string Movie::getFormat() { return format; }
const string Movie::getRating() { return rating; }
const string Movie::getDirector() { return director; }
const string Movie::getActors() { return actors; }
const string Movie::getTrailerURL() { return trailer_url; }
const string Movie::getGenre() { return genre; }
const string Movie::getLanguage() { return language; }
const string Movie::getProducer() { return producer; }
const string Movie::getCountry() { return country; }
const Date Movie::getLaunchDate() { return launch_date; }

void Movie::setName(const string& newName) { name = newName; };
void Movie::setFormat(const string& newFormat) { format = newFormat; }
void Movie::setRating(const string& newRating) { rating = newRating; }
void Movie::setDirector(const string& newDirector) { director = newDirector; }
void Movie::setActors(const string& newActors) { actors = newActors; }
void Movie::setTrailerURL(const string& newTrailerURL) { trailer_url = newTrailerURL; }
void Movie::setGenre(const string& newGenre) { genre = newGenre; }
void Movie::setLanguage(const string& newLanguage) { language = newLanguage; }
void Movie::setProducer(const string& newProducer) { producer = newProducer; }
void Movie::setCountry(const string& newCountry) { country = newCountry; }
void Movie::setLaunchDate(const Date& newLaunchDate) { launch_date = newLaunchDate; }

void MovieInterface::displayMovie(Movie& movie) {
	cout << movie.getName() << endl;
	cout << "Format: " << movie.getFormat() << endl;
	cout << "Rating: " << movie.getRating() << endl;
	cout << "Gnenre: " << movie.getGenre() << endl;
	cout << "Trailer: " << movie.getTrailerURL() << endl;
}

void MovieInterface::displayMovieExtended(Movie& movie) {
	cout << "Original title: " << movie.getName() << endl;
	cout << "Director: " << movie.getDirector() << endl;
	cout << "Actors: " << movie.getActors() << endl;
	cout << "Gnenre: " << movie.getGenre() << endl;
	cout << "Language: " << movie.getLanguage() << endl;
	cout << "Format: " << movie.getFormat() << endl;
	cout << "Launch date: " << movie.getLaunchDate() << endl;
	cout << "Country: " << movie.getCountry() << endl;
	cout << "Producer: " << movie.getProducer() << endl;
	cout << "Rating: " << movie.getRating() << endl;
	cout << "Trailer: " << movie.getTrailerURL() << endl;
}

Movie MovieInterface::displayUniqueMovies() {
	MovieRepository movieRep;
	Movie* movies = nullptr;
	Movie movie;
	int number = movieRep.getNumberOfTotalUniqueMovies();
	int count = 0;
	int option;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT m1.*, YEAR(launch_date) AS launch_date_year, MONTH(launch_date) as launch_date_month, DAY(launch_date) as launch_date_day FROM movies m1 WHERE m1.id = (SELECT MIN(m2.id) FROM movies m2 WHERE m2.name = m1.name);");
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()) {
			movie.setName(res->getString("name"));
			movie.setFormat(res->getString("format"));
			movie.setRating(res->getString("rating"));
			movie.setDirector(res->getString("director"));
			movie.setActors(res->getString("actors"));
			movie.setTrailerURL(res->getString("trailer_url"));
			movie.setGenre(res->getString("genre"));
			movie.setLanguage(res->getString("language"));
			movie.setProducer(res->getString("producer"));
			movie.setCountry(res->getString("country"));
			movie.setLaunchDate(Date(res->getInt("launch_date_year"),
				res->getInt("launch_date_month"),
				res->getInt("launch_date_day"))
			);
			if (count == 0) {
				movies = new Movie[number];
			}
			movies[count] = movie;
			this->displayMovie(movie);
			cout << "Option: " << ++count << endl;
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
		cerr << "Could not display unique movies. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);

	Movie selectedMovie = movies[option - 1];
	delete[] movies;
	return selectedMovie;
}

Date MovieInterface::displayDates(Movie& movie, Auditorium& auditorium, Theatre& theatre) {
	AuditoriumRepository auditoriumRep;
	TheatreRepository theatreRep;
	MovieRepository movieRep;
	Date* dates = nullptr;
	int number = movieRep.getNumberOfDatesByMovieANDTheatre(movie, auditorium, theatre);
	int count = 0;
	int option;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT YEAR(movie_broadcast.broadcast_date) as broadcast_year, MONTH(movie_broadcast.broadcast_date) as broadcast_month, DAY(movie_broadcast.broadcast_date) as broadcast_day, HOUR(movie_broadcast.broadcast_date) as broadcast_hour, MINUTE(movie_broadcast.broadcast_date) as broadcast_minute from movie_broadcast JOIN movies ON movies.id = movie_broadcast.movie_id WHERE theatre_id = ? AND auditorium_id = ? AND name = ? AND broadcast_date > CURRENT_TIMESTAMP;");
		pstmt->setInt(1, theatreRep.getTheatreID(theatre));
		pstmt->setInt(2, auditoriumRep.getAuditoriumID(auditorium, theatre));
		pstmt->setString(3, movie.getName());
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()){
			Date date(res->getInt("broadcast_year"), res->getInt("broadcast_month"), res->getInt("broadcast_day"), res->getInt("broadcast_hour"), res->getInt("broadcast_minute"));
			if (count == 0) {
				dates = new Date[number];
			}
			dates[count] = date;
			date.displayDateHoursMinutes();
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
		cerr << "Could not display dates by movie and theatre. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);

	Date selectedDate = dates[option - 1];
	delete[] dates;
	return selectedDate;
}

Movie MovieInterface::readMovie() {
	Movie movie;
	string name, format, rating, director, actors, trailer_url, genre, language, producer, country;
	Date launch_date;

	do {
		cin.ignore();

		cout << "Enter Movie Details:" << endl;

		cout << "Name: "; getline(cin, name);
		if (name.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Name exceeds maximum length." << endl;
			continue;
		}
		movie.setName(name);

		cout << "Format: "; getline(cin, format);
		if (format.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Format exceeds maximum length." << endl;
			continue;
		}
		movie.setFormat(format);

		cout << "Rating: "; getline(cin, rating);
		if (rating.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Rating exceeds maximum length." << endl;
			continue;
		}
		movie.setRating(rating);

		cout << "Director: "; getline(cin, director);
		if (director.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Director exceeds maximum length." << endl;
			continue;
		}
		movie.setDirector(director);

		cout << "Actors: "; getline(cin, actors);

		cout << "Trailer URL: "; getline(cin, trailer_url);
		if (trailer_url.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Trailer URL exceeds maximum length." << endl;
			continue;
		}
		movie.setTrailerURL(trailer_url);

		cout << "Genre: "; getline(cin, genre);
		if (genre.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Genre exceeds maximum length." << endl;
			continue;
		}
		movie.setGenre(genre);

		cout << "Language: "; getline(cin, language);
		if (language.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Language exceeds maximum length." << endl;
			continue;
		}
		movie.setLanguage(language);

		cout << "Producer: "; getline(cin, producer);
		if (producer.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Producer exceeds maximum length." << endl;
			continue;
		}
		movie.setProducer(producer);

		cout << "Country: "; getline(cin, country);
		if (country.length() > MAX_STRING_LENGHT) {
			cerr << "Error: Country exceeds maximum length." << endl;
			continue;
		}
		movie.setCountry(country);

		cout << "Launch Date (YYYY-MM-DD): "; cin >> launch_date;
		movie.setLaunchDate(launch_date);

		break;

	} while (true);

	return movie;
}


int MovieRepository::getMovieID(Movie& movie, Auditorium& auditorium, Theatre& theatre) {
	sql::Connection* con = dbConnector.establishConnection();
	int id;
	try {
		TheatreRepository theatreRep;
		int theatre_id = theatreRep.getTheatreID(theatre);
		AuditoriumRepository auditoriumRep;
		int auditorium_id = auditoriumRep.getAuditoriumID(auditorium, theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id FROM movies WHERE name = ? AND theatre_id = ? AND auditorium_id = ?;");
		pstmt->setString(1, movie.getName());
		pstmt->setInt(2, theatre_id);
		pstmt->setInt(3, auditorium_id);
		sql::ResultSet* res = pstmt->executeQuery();
		if (res->next()) {
			id = res->getInt("id");
		}
		else cout << "No data found. " << endl;
		
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get movie id. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return id;
}

int MovieRepository::getNumberOfTotalUniqueMovies() {
	sql::Connection* con = dbConnector.establishConnection();
	int number = 0;
	try {
		sql::Statement* stmt = con->createStatement();
		sql::ResultSet* res = stmt->executeQuery("SELECT SUM(unique_movie_count) AS total_unique_movies FROM( SELECT COUNT(DISTINCT name) AS unique_movie_count FROM movies GROUP BY name) AS counts;");
		if (res->next()) {
			number = res->getInt("total_unique_movies");
		}
		else {
			cerr << "No data found in the result set." << endl;
		}
		delete stmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get number of unique movies by name. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return number;
}

int MovieRepository::getNumberOfDatesByMovieANDTheatre(Movie& movie, Auditorium& auditorium, Theatre& theatre) {
	TheatreRepository theatreRep;
	AuditoriumRepository auditoriumRep;
	int number = 0;
	sql::Connection* con = dbConnector.establishConnection();
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT count(broadcast_date) as total FROM movie_broadcast JOIN movies ON movies.id = movie_broadcast.movie_id WHERE theatre_id = ? AND auditorium_id = ? AND name = ? AND broadcast_date > CURRENT_TIMESTAMP;");
		pstmt->setInt(1, theatreRep.getTheatreID(theatre));
		cout << "Asdas: " << auditoriumRep.getAuditoriumID(auditorium, theatre) << endl;
		pstmt->setInt(2, auditoriumRep.getAuditoriumID(auditorium, theatre));
		pstmt->setString(3, movie.getName());
		sql::ResultSet* res = pstmt->executeQuery();
		if (res->next()) {
			number = res->getInt("total");
		}
		else {
			cerr << "No data found in the result set." << endl;
		}
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not get number of dates by movie and theatre. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return number;
}

void MovieRepository::insertIntoDatabase(Movie& movie, Theatre& theatre, Auditorium& auditorium) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TheatreRepository theatreRep;
		int theatre_id = theatreRep.getTheatreID(theatre);
		AuditoriumRepository auditoriumRep;
		int auditorium_id = auditoriumRep.getAuditoriumID(auditorium, theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO movies (theatre_id, auditorium_id, name, format, rating, director, actors, trailer_url, genre, language, producer, country, launch_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
		Date launchDate = movie.getLaunchDate();
		string launchDateString = to_string(launchDate.getYear()) + "-" + to_string(launchDate.getMonth()) + "-" + to_string(launchDate.getDay());
		pstmt->setInt(1, theatre_id);
		pstmt->setInt(2, auditorium_id);
		pstmt->setString(3, movie.getName());
		pstmt->setString(4, movie.getFormat());
		pstmt->setString(5, movie.getRating());
		pstmt->setString(6, movie.getDirector());
		pstmt->setString(7, movie.getActors());
		pstmt->setString(8, movie.getTrailerURL());
		pstmt->setString(9, movie.getGenre());
		pstmt->setString(10, movie.getLanguage());
		pstmt->setString(11, movie.getProducer());
		pstmt->setString(12, movie.getCountry());
		pstmt->setString(13, launchDateString);

		pstmt->executeUpdate();
		delete pstmt;
		cout << "Movie added successfuly into database." << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not insert the movie into database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

void MovieRepository::deleteFromDatabase(Movie& movie, Theatre& theatre, Auditorium& auditorium) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TheatreRepository theatreRep;
		int theatre_id = theatreRep.getTheatreID(theatre);
		AuditoriumRepository auditoriumRep;
		int auditorium_id = auditoriumRep.getAuditoriumID(auditorium, theatre);
		sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM movies WHERE theatre_id = ? AND auditorium_id = ? AND name = ?;");
		pstmt->setInt(1, theatre_id);
		pstmt->setInt(2, auditorium_id);
		pstmt->setString(3, movie.getName());
		pstmt->executeUpdate();
		delete pstmt;
		cout << "Movie deleted successfuly from database." << endl;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not delete the movie from database. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

Movie MovieRepository::loadMovie(int movieID) {
	sql::Connection* con = dbConnector.establishConnection();
	Movie movie;
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT .*, YEAR(launch_date) AS launch_date_year, MONTH(launch_date) as launch_date_month, DAY(launch_date) as launch_date_day FROM movies WHERE id = ?);");
		pstmt->setInt(1, movieID);
		sql::ResultSet* res = pstmt->executeQuery();
		if (res->next()) {
			movie.setName(res->getString("name"));
			movie.setFormat(res->getString("format"));
			movie.setRating(res->getString("rating"));
			movie.setDirector(res->getString("director"));
			movie.setActors(res->getString("actors"));
			movie.setTrailerURL(res->getString("trailer_url"));
			movie.setGenre(res->getString("genre"));
			movie.setLanguage(res->getString("language"));
			movie.setProducer(res->getString("producer"));
			movie.setCountry(res->getString("country"));
			movie.setLaunchDate(Date(res->getInt("launch_date_year"), 
									 res->getInt("launch_date_month"), 
									 res->getInt("launch_date_day"))
			);
		}
		else {
			cerr << "Movie not found." << endl;
		}
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not load movie. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return movie;
}

Movie MovieRepository::loadMovieByName(const string& name) {
	sql::Connection* con = dbConnector.establishConnection();
	Movie movie;
	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT .*, YEAR(launch_date) AS launch_date_year, MONTH(launch_date) as launch_date_month, DAY(launch_date) as launch_date_day FROM movies WHERE name = ?);");
		pstmt->setString(1, name);
		sql::ResultSet* res = pstmt->executeQuery();
		if (res->next()) {
			movie.setName(res->getString("name"));
			movie.setFormat(res->getString("format"));
			movie.setRating(res->getString("rating"));
			movie.setDirector(res->getString("director"));
			movie.setActors(res->getString("actors"));
			movie.setTrailerURL(res->getString("trailer_url"));
			movie.setGenre(res->getString("genre"));
			movie.setLanguage(res->getString("language"));
			movie.setProducer(res->getString("producer"));
			movie.setCountry(res->getString("country"));
			movie.setLaunchDate(Date(res->getInt("launch_date_year"),
				res->getInt("launch_date_month"),
				res->getInt("launch_date_day"))
			);
		}
		else {
			cerr << "Movie not found." << endl;
		}
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not load movie. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
	return movie;
}

template <>
void MovieRepository::updateMovie(Movie& movie, const string& field, const Date& value) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		const string query = "UPDATE movies SET " + field + " = ? WHERE name = ?";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setString(1, value.toString());
		pstmt->setString(2, movie.getName());
		int rowsUpdated = pstmt->executeUpdate();

		if (rowsUpdated > 0) {
			cout << "Movie" << movie.getName() << " modified with success." << endl;
			con->commit();
		}
		else {
			cout << "Movie" << movie.getName() << " not found." << endl;
		}
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not update movie ( " << movie.getName() << "). Error message : " << e.what() << endl;
		con->rollback();
		exit(1);
	}
	dbConnector.closeConnection(con);
}

template <>
void MovieRepository::updateMovie(Movie& movie, const string& field, const string& value) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		const string query = "UPDATE movies SET " + field + " = ? WHERE name = ?";
		sql::PreparedStatement* pstmt = con->prepareStatement(query);
		pstmt->setString(1, value);
		pstmt->setString(2, movie.getName());
		int rowsUpdated = pstmt->executeUpdate();

		if (rowsUpdated > 0) {
			cout << "Movie" << movie.getName() << " modified with success." << endl;
			con->commit();
		}
		else {
			cout << "Movie" << movie.getName() << " not found." << endl;
		}
		delete pstmt;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not update movie ( " << movie.getName() << "). Error message : " << e.what() << endl;
		con->rollback();
		exit(1);
	}
	dbConnector.closeConnection(con);
}