#pragma once
#include "auditorium.h"
#include "teathre.h"

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
	Movie(const string& name, const string& format, const string& rating, const string& director, const string& actors, const string& trailer_url, const string& genre, const string& language, const string& producer, const string& country, const Date& launch_date);
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
public:
	void displayMovie(Movie& movie);
	void displayMovieExtended(Movie& movie);
};

class MovieRepository {
private:
	DatabaseConnector dbConnector;
public:
	MovieRepository() {}
	int getMovieID(Movie& movie, Auditorium& auditorium, Teathre& teathre);
	void insertIntoDatabase(Movie& movie, Teathre& teathre, Auditorium& auditorium);
	//Movie loadMovie();
	template <typename T>
	void updateMovie(Movie& movie, const string& field, const T& value);
};
