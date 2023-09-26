#pragma once

#include <string>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using std::string;

class DatabaseConnector {
private:
    const string server = "tcp://127.0.0.1:3306";
    const string username = "root";
    const string password = "1234";
    const string database = "movie_ticket_booking_database";

public:
    sql::Connection* establishConnection();
    void closeConnection(sql::Connection* con);
};
