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

void MovieInterface::displayUniqueMovies() {
	sql::Connection* con = dbConnector.establishConnection();
	Movie movie;
	int count = 0;
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
			this->displayMovie(movie);
			cout << "Option: " << ++count << endl;
		}
		delete pstmt;
		delete res;
	}
	catch (sql::SQLException& e) {
		cerr << "Could not load movie. Error: " << e.what() << endl;
		exit(1);
	}
	dbConnector.closeConnection(con);
}

int MovieRepository::getMovieID(Movie& movie, Auditorium& auditorium, Teathre& teathre) {
	sql::Connection* con = dbConnector.establishConnection();
	int id;
	try {
		TeathreRepository teathreRep;
		int teathre_id = teathreRep.getTeathreID(teathre);
		AuditoriumRepository auditoriumRep;
		int auditorium_id = auditoriumRep.getAuditoriumID(auditorium, teathre);
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT id FROM movies WHERE name = ? AND teathre_id = ? AND auditorium_id = ?;");
		pstmt->setString(1, movie.getName());
		pstmt->setInt(2, teathre_id);
		pstmt->setInt(3, auditorium_id);
		sql::ResultSet* res = pstmt->executeQuery();
		id = res->getInt("id");
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

void MovieRepository::insertIntoDatabase(Movie& movie, Teathre& teathre, Auditorium& auditorium) {
	sql::Connection* con = dbConnector.establishConnection();
	try {
		TeathreRepository teathreRep;
		int teathre_id = teathreRep.getTeathreID(teathre);
		AuditoriumRepository auditoriumRep;
		int auditorium_id = auditoriumRep.getAuditoriumID(auditorium, teathre);
		sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO movies (teathre_id, auditorium_id, name, format, rating, director, actors, trailer_url, genre, language, producer, country, launch_date) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
		Date launchDate = movie.getLaunchDate();
		string launchDateString = to_string(launchDate.getYear()) + "-" + to_string(launchDate.getMonth()) + "-" + to_string(launchDate.getDay());
		pstmt->setInt(1, teathre_id);
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


template <typename T>
void MovieRepository::updateMovie(Movie& movie, const string& field, const T& value) {
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
