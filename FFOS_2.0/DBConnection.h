#pragma once
#ifndef DBConnection_H
#define DBConnection_H

#include <mysql/jdbc.h> 

#include <string> 


class DBConnection {
private:
	sql::Connection* connection;
	sql::Statement* query;
	bool connected;


public:
	sql::ResultSet* res;
	DBConnection();
	~DBConnection();
	void QueryResult(std::string statement);
	void QueryStatement(std::string statement);

};


#endif // !DBConnection_H

