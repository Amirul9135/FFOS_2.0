#include "DBConnection.h";
#include <conio.h>
using namespace sql;
using namespace std;

DBConnection::DBConnection() {
	try {
		mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();
		connection = driver->connect("tcp://127.0.0.1:3306", "admin", "qwertyu");

		connection->setSchema("ffosdb");


		query = connection->createStatement();
		connected = true;
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() == 0) {
			system("cls");
			cout << "Unable to connect to database";
			_getch();
			exit(0);
			return;
		}
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}
}

DBConnection::~DBConnection() {
	try {
		if(connected)
			connection->close();
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() != 0) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			_getch();
		}
	}
}

void DBConnection::QueryStatement(string statement) {

	try {
		if(connected)
			query->execute(statement);
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() != 0) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			_getch();
		}
	}
}

void DBConnection::QueryResult(string statement) {

	try {
		if(connected)
			res = query->executeQuery(statement);
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() != 0) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			_getch();
		}
	}
}

