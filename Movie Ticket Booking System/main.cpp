#include "user.h"
#include "public_user.h"
#include "login.h"

int main() {
	//User a("", " ", "email@example.com", "1234");
	//UserRepository b;
	//b.listUsersByName("Simion", "Dragos-Ionut");
	//b.updateUser(a, "firstName", "Dragos Ionut");
	
	//PublicUserRepository bb;
	//PublicUser aa = bb.loadPublicUser("email@example.com", "1234");
	//PublicUser aa;
	//aa.read();
	//PublicUserRepository bb;
	//bb.insertIntoDatabase(aa);
	
	Login login;
	LoginRepository loginRep;
	LoginInterface loginInt;

	loginInt.displayLoginMenu(login, loginRep);
}