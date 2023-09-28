#include "login.h"
#include "movie.h"
#include "auditorium.h"
#include "teathre.h"
#include "public_user.h"
#include "private_user.h"

const string CINEMA_NAME = "Vision Cinema";

int displayMainOptions() {
	int option;
	cout << CINEMA_NAME << " | 1 - Movies | 2 - Tariffs | 3 - Login | 4 - Exit |" << endl;
	cout << "Enter option: "; cin >> option;
	while (option < 1 || option > 4) {
		cout << "Invalid option, retrying..." << endl;
			cout << "Enter option: "; cin >> option;
	}
	return option;
}

void displayProgram() {
	cout << "Ticket type:                                   2D MOVIE                               3D MOVIE            " << endl;
	cout << "----------------------------------------------------------------------------------------------------------" << endl;
	cout << "                                 Monday - Thursday   Friday - Sunday   Monday - Thursday   Friday - Sunday" << endl;
	cout << "Adults until 17:00                    25 LEI              28 LEI            26 LEI              31 LEI    " << endl;
	cout << "Adults after 17:00                    28 LEI              30 LEI            29 LEI              33 LEI    " << endl;
	cout << "Pupils, students, pensioners          22 LEI              24 LEI            24 LEI              27 LEI    " << endl;
	cout << "Child under 12 years                  18 LEI              20 LEI            20 LEI              22 LEI    " << endl;
	cout << "Family 2 pers*(1 adult + 1 child)     36 LEI              40 LEI            40 LEI              44 LEI    " << endl;
	cout << "Family 3 pers*(2 adult + 1 child)     54 LEI              60 LEI            60 LEI              66 LEI    " << endl;
	cout << "Family 4 pers*(3 adult + 1 child)     72 LEI              80 LEI            80 LEI              88 LEI    " << endl;
	cout << "Family 4 pers*(4 pupils/students)     78 LEI                -               84 LEI                -       " << endl;
	cout << "Preview                                         35 LEI                                 35 LEI             " << endl;
	cout << "----------------------------------------------------------------------------------------------------------" << endl;
}

int main() {

	Login login;
	LoginRepository loginRep;
	LoginInterface loginInt;

	Movie movie;
	MovieRepository movieRep;
	MovieInterface movieInt;

	bool loggedIn = false;
	bool exitMenu = false;

	while (!exitMenu) {
		switch (displayMainOptions()) {
		case 1:
			int option;
			movieInt.displayUniqueMovies();
			cout << "Enter option between 1 and " << movieRep.getNumberOfTotalUniqueMovies() << ":"; cin >> option;
			while (option < 1 || option > 4) {
				cout << "Invalid option, retrying..." << endl;
				cout << "Enter option: "; cin >> option;
			}
			switch (option) {
				
			}
			break;
		case 2:
			displayProgram();
			break;
		case 3:
			if (loginInt.displayLoginMenu(login, loginRep)) 
				loggedIn = true;
			break;
		case 4:
			exitMenu = true;
			cout << "Exited with success!" << endl;
		}
	}
}