#include "login.h"
#include "movie.h"
const string CINEMA_NAME = "Vision Cinema";

int displayMainPage() {
	int option;
	cout << CINEMA_NAME << " | 1 - Movies | 2 - Program | 3 - Tariffs | 4 - Login" << endl;
	
	return option;
}

int main() {

	Login login;
	LoginRepository loginRep;
	LoginInterface loginInt;

	MovieRepository movieRep;
	int nr = movieRep.getNumberOfTotalUniqueMovies();
	cout << nr;
	//loginInt.displayLoginMenu(login, loginRep);
}