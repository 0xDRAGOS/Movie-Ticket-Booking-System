#include "users.h"

int main() {
	User a("", " ", "email@example.com", "1234");
	UserRepository b;
	//b.listUsersByName("Simion", "Dragos-Ionut");
	//b.updateUser(a, "firstName", "Dragos Ionut");
	
	PublicUserRepository bb;
	PublicUser aa = bb.loadPublicUser("email@example.com", "1234");
}