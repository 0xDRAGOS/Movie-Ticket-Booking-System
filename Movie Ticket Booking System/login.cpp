#include "login.h"

Login::Login(const string& inputEmail, const string& inputPassword) : inputEmail(inputEmail), inputPassword(inputPassword) {}

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