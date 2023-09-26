#include "database_connection.h"
using std::cerr;
using std::endl;

sql::Connection* DatabaseConnector::establishConnection() {
    sql::Connection* con;
    sql::Driver* driver;

    try {
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        con->setSchema(database);
    }
    catch (sql::SQLException& e) {
        cerr << "Could not connect to server. Error message: " << e.what() << endl;
        exit(1);
    }

    return con;
}

void DatabaseConnector::closeConnection(sql::Connection* con) {
    if (con) {
        delete con;
    }
}