#include "login.h"

Login::Login(const string& inputEmail, const string& inputPassword) 
    : inputEmail(inputEmail), inputPassword(inputPassword) {}

const string Login::getInputEmail() { return inputEmail; };
const string Login::getInputPassword() { return inputPassword; };

bool LoginRepository::validateLogin(Login& login, UserType type) {
    sql::Connection* con = dbConnector.establishConnection();
    bool isValid = false;
    string query;

    if (type == Public) query = "SELECT COUNT(*) FROM users WHERE email = ? AND password = ? AND user_type = 'public';";
    else query = "SELECT COUNT(*) FROM users WHERE email = ? AND password = ? AND user_type = 'private';";

    try {
        sql::PreparedStatement* pstmt = con->prepareStatement(query);
        pstmt->setString(1, login.getInputEmail());
        pstmt->setString(2, login.getInputPassword());
        sql::ResultSet* res = pstmt->executeQuery();
        if (res->next() && res->getInt(1) == 1) {
            isValid = true;
        }
        delete pstmt;
        delete res;
    }
    catch (sql::SQLException& e) {
        cerr << "Could not validate logn. Error: " << e.what() << endl;
    }
    dbConnector.closeConnection(con);
    return isValid;
}



PublicUser LoginInterface::displayLoginMenuPublicUser(Login& login, LoginRepository& loginRep) {
    string inputEmail, inputPassword;
    int loginAttempts = 0;
    bool isValid = false;
    cout << "^----------[LOGIN]----------^" << endl;
    while (!isValid && loginAttempts < MAX_LOGIN_ATTEMPTS) {
        cout << "| Enter email: "; cin >> inputEmail;
        cout << "| Enter password: "; cin >> inputPassword;

        login = Login(inputEmail, inputPassword);
        if (isValid = loginRep.validateLogin(login, Public)) {
            cout << "Logged in successfully as an usual user!" << endl;
        }
        else {
            cout << "Wrong email/password, retrying..." << endl;
            loginAttempts++;
        }
    }
    if (loginAttempts == MAX_LOGIN_ATTEMPTS) cout << "Maximum login attempts reached. Exiting... " << endl;
    cout << "_____________________________" << endl;
    
    if (isValid && loginAttempts < MAX_LOGIN_ATTEMPTS) {
        PublicUserRepository publicUserRep;
        PublicUser publicUser = publicUserRep.loadPublicUser(login.getInputEmail(), login.getInputPassword());
        return publicUser;
    }
}

PrivateUser LoginInterface::displayLoginMenuPrivateUser(Login& login, LoginRepository& loginRep) {
    string inputEmail, inputPassword;
    int loginAttempts = 0;
    bool isValid = false;
    cout << "^----------[LOGIN]----------^" << endl;
    while (!isValid && loginAttempts < MAX_LOGIN_ATTEMPTS) {
        cout << "| Enter email: "; cin >> inputEmail;
        cout << "| Enter password: "; cin >> inputPassword;

        login = Login(inputEmail, inputPassword);
        if (isValid = loginRep.validateLogin(login, Private)) {
            cout << "Logged in successfully as an administrator!" << endl;
        }
        else {
            cout << "Wrong email/password, retrying..." << endl;
            loginAttempts++;
        }
    }
    if (loginAttempts == MAX_LOGIN_ATTEMPTS) cout << "Maximum login attempts reached. Exiting... " << endl;
    cout << "_____________________________" << endl;
    
    if (isValid && loginAttempts < MAX_LOGIN_ATTEMPTS) {
        PrivateUserRepository privateUserRep;
        PrivateUser privateUser = privateUserRep.loadPrivateUser(login.getInputEmail(), login.getInputPassword());
        return privateUser;
    }
}