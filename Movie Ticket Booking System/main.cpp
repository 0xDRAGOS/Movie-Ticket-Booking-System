#include "login.h"

int main() {

	Login login;
	LoginRepository loginRep;
	LoginInterface loginInt;

	loginInt.displayLoginMenu(login, loginRep);
}