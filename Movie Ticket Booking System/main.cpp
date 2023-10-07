#include "login.h"
#include "ticket.h"
#include "movie.h"
#include "auditorium.h"
#include "theatre.h"
#include "public_user.h"
#include "private_user.h"

const string CINEMA_NAME = "Vision Cinema";

bool loggedInPublicUser = false;
bool loggedInPrivateUser = false;

Login login;
LoginRepository loginRep;
LoginInterface loginInt;

PublicUserRepository publicUserRep;
PublicUser loadedPublicUser;

PrivateUser loadedPrivateUser;
PrivateUserInterface privateUserInt;

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

void displayTariffs() {
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

void loginMenu() {
	int login_option;
	if (!loggedInPublicUser || !loggedInPrivateUser) {
		cout << "Login in as an usual user / Login as an administrator (1/2)" << endl;
		cout << "Enter option: ";  cin >> login_option;
		while (login_option < 1 || login_option > 2) {
			cout << "Invalid option, retrying..." << endl;
			cout << "Enter option: ";  cin >> login_option;
		}
		switch (login_option) {
		case 1:
			loadedPublicUser = loginInt.displayLoginMenuPublicUser(login, loginRep);
			if (loadedPublicUser.getLoggedInStatus()) {
				loggedInPublicUser = true;
				loggedInPrivateUser = false;
			}
			break;
		case 2:
			loadedPrivateUser = loginInt.displayLoginMenuPrivateUser(login, loginRep);
			if (loadedPrivateUser.getLoggedInStatus()) {
				loggedInPublicUser = false;
				loggedInPrivateUser = true;
			}
			break;
		}
	}
	else cout << "You are already logged in. " << endl;
}

int main() {
	Date selectedDate;

	Auditorium auditorium;
	Auditorium selectedAuditorium;
	AuditoriumRepository auditoriumRep;
	AuditoriumInterface auditoriumInt;

	Theatre theatre;
	Theatre selectedTheatre;
	TheatreRepository theatreRep;
	TheatreInterface theatreInt;

	Movie movie;
	Movie selectedMovie;
	MovieRepository movieRep;
	MovieInterface movieInt;

	Ticket ticket;
	TicketRepository ticketRep;
	TicketInterface ticketInt;

	bool exitMenu = false;

	while (!exitMenu) {
		switch (displayMainOptions()) {
		case 1:
			selectedMovie = movieInt.displayUniqueMovies();
			cout << "----------------------------------------------------------------------------------------------------------" << endl;
			movieInt.displayMovieExtended(selectedMovie);
			selectedTheatre = theatreInt.displayTheatres();
			selectedAuditorium = auditoriumInt.displayAuditoriums(selectedMovie, selectedTheatre);
			selectedDate = movieInt.displayDates(selectedMovie, selectedAuditorium, selectedTheatre);

			loginMenu();

			if (loggedInPublicUser || loggedInPrivateUser) {
				auditoriumInt.displayAuditoriumSeats(selectedAuditorium);
				auditoriumInt.displaySelectSeat(selectedAuditorium);
				ticket = ticketInt.displayCalculateTicketPriceBasedOnOptions();
				if (ticketRep.insertIntoDatabase(ticket, loadedPublicUser, selectedMovie, selectedAuditorium, selectedTheatre)) {
					cout << "Your ticket has been successfully registered." << endl;
				}
			}
			break;
		case 2:
			displayTariffs();
			break;
		case 3:
			loginMenu();

			if (loggedInPrivateUser) {
				bool exitPrivateUserMenu = false;
				while (!exitPrivateUserMenu) {
					switch (privateUserInt.displayMainMenu()) {
					case 1:
					{
						string email, password;
						cout << "Enter email: "; cin >> email;
						cout << "Enter password: "; cin >> password;
						PublicUser user = publicUserRep.loadPublicUser(email, password);
						loadedPrivateUser.updateCredentials(user);
					}
					case 2:
					{
						string name;
						cout << "Enter name: "; getline(cin, name);
						Theatre theatre = theatreRep.loadTheatre(name);
						loadedPrivateUser.updateTheatre(theatre);
					}
					case 3:
					{
						string name;
						Address address;
						cout << "Enter name: "; getline(cin, name);
						cout << "Enter address: "; cin >> address;
						theatre.setName(name);
						theatre.setAddress(address);
						loadedPrivateUser.addTheatre(theatre);
					}
					case 4:
					{
						string name;
						cout << "Enter name: "; getline(cin, name);
						theatre = theatreRep.loadTheatre(name);
						loadedPrivateUser.deleteTheatre(theatre);
					}
					case 5:
					{
						string name;
						cout << "Enter theatre name: "; getline(cin, name);
						theatre = theatreRep.loadTheatre(name);
						auditorium = auditoriumInt.readAuditorium();
						loadedPrivateUser.updateAuditorium(auditorium, theatre);
					}
					case 6:
					{
						string name;
						cout << "Enter theatre name: "; getline(cin, name);
						theatre = theatreRep.loadTheatre(name);
						auditorium = auditoriumInt.readAuditorium();
						loadedPrivateUser.addAuditorium(auditorium, theatre);
					}
					case 7:
					{
						string name;
						cout << "Enter theatre name: "; getline(cin, name);
						theatre = theatreRep.loadTheatre(name);
						auditorium = auditoriumInt.readAuditorium();
						loadedPrivateUser.deleteAuditorium(auditorium, theatre);
					}
					case 8:
					{
						string name;
						cout << "Enter name: "; getline(cin, name);
						movie = movieRep.loadMovieByName(name);
						loadedPrivateUser.updateMovieDetails(movie);
					}
					case 9:
					{
						string name;
						cout << "Enter theatre name: "; getline(cin, name);
						theatre = theatreRep.loadTheatre(name);
						auditorium = auditoriumInt.readAuditorium();
						movie = movieInt.readMovie();
						loadedPrivateUser.addMovie(movie, auditorium, theatre);
					}
					case 10:
					{
						string name;
						cout << "Enter theatre name: "; getline(cin, name);
						theatre = theatreRep.loadTheatre(name);
						auditorium = auditoriumInt.readAuditorium();
						movie = movieInt.readMovie();
						loadedPrivateUser.deleteMovie(movie, auditorium, theatre);
					}
					case 11:
					{
						exitPrivateUserMenu = true;
						cout << "Exited with success!" << endl;
						break;
					}
					}
				}
			}
			break;
		case 4:
			exitMenu = true;
			cout << "Exited with success!" << endl;
			break;
		}
	}
}