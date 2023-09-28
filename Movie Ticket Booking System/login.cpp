#include "login.h"

Login::Login(const string& inputEmail, const string& inputPassword) 
    : inputEmail(inputEmail), inputPassword(inputPassword) {}

const string Login::getInputEmail() { return inputEmail; };
const string Login::getInputPassword() { return inputPassword; };

bool LoginRepository::validateLogin(Login& login) {
    sql::Connection* con = dbConnector.establishConnection();
    bool isValid = false;
    try {
        sql::PreparedStatement* pstmt = con->prepareStatement("SELECT COUNT(*) FROM users WHERE email = ? AND password = ?");
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
        cerr << "Error while validating login: " << e.what() << endl;
    }
    dbConnector.closeConnection(con);
    return isValid;
}

bool LoginInterface::displayLoginMenu(Login& login, LoginRepository& loginRep) {
    string inputEmail, inputPassword;
    bool isValid = false;
    cout << "^----------[LOGIN]----------^" << endl;
    while (!isValid) {
        cout << "| Enter email: "; cin >> inputEmail;
        cout << "| Enter password: "; cin >> inputPassword;

        login = Login(inputEmail, inputPassword);
        isValid = loginRep.validateLogin(login);
        if (!isValid) cout << "Wrong email/password, retrying..." << endl;
    }
    if (isValid) cout << "Logged in successfully!" << endl;
    cout << "_____________________________" << endl;
    return isValid;
}