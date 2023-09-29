#pragma once
#include "auditorium.h"
#include "theatre.h"
#include "date.h"

class Auditorium;
class Theatre;

class Movie {
private:
	string name;
	string format;
	string rating;
	string director;
	string actors;
	string trailer_url;
	string genre;
	string language;
	string producer;
	string country;
	Date launch_date;
public:
	Movie(const string& name = "", const string& format = "", const string& rating = "", const string& director = "", const string& actors = "", const string& trailer_url = "", const string& genre = "", const string& language = "", const string& producer = "", const string& country = "", const Date& launch_date = Date());
	const string getName();
	const string getFormat();
	const string getRating();
	const string getDirector();
	const string getActors();
	const string getTrailerURL();
	const string getGenre();
	const string getLanguage();
	const string getProducer();
	const string getCountry();
	const Date getLaunchDate();
	void setName(const string& newName);
	void setFormat(const string& newFormat);
	void setRating(const string& newRating);
	void setDirector(const string& newDirector);
	void setActors(const string& newActors);
	void setTrailerURL(const string& newTrailerURL);
	void setGenre(const string& newGenre);
	void setLanguage(const string& newLanguage);
	void setProducer(const string& newProducer);
	void setCountry(const string& newCountry);
	void setLaunchDate(const Date& newLaunchDate);
};

class MovieInterface {
private:
	DatabaseConnector dbConnector;
public:
	void displayMovie(Movie& movie);
	void displayMovieExtended(Movie& movie);
	Movie displayUniqueMovies();
	Date displayDateByMovieANDTheatre(Movie& movie, Theatre& theatre);
};

class MovieRepository {
private:
	DatabaseConnector dbConnector;
public:
	MovieRepository() {}
	int getMovieID(Movie& movie, Auditorium& auditorium, Theatre& theatre);
	int getNumberOfTotalUniqueMovies();
	int getNumberOfDatesByMovieANDTheatre(Movie& movie, Theatre& theatre);
	void insertIntoDatabase(Movie& movie, Theatre& theatre, Auditorium& auditorium);
	Movie loadMovie(int movieID);
	template <typename T>
	void updateMovie(Movie& movie, const string& field, const T& value);
};
