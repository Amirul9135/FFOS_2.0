#include "Account.h";
#include "DBConnection.h";
#include <mysql/jdbc.h> 

using namespace sql;
using namespace std;

Account::Account() {
}


void Account::UpdateName() {
	DBConnection db;
	db.QueryStatement("UPDATE accounts SET Name ='" + Name + "' WHERE IC='" + IC + "'");
	db.~DBConnection();
}

void Account::UpdatePassword() {
	DBConnection db;
	db.QueryStatement("UPDATE accounts SET Password ='" + Password + "' WHERE IC='" + IC + "'");
	db.~DBConnection();
}

void Account::UpdatePhoneNo() {
	DBConnection db;
	db.QueryStatement("UPDATE accounts SET PhoneNo ='" + PhoneNo + "' WHERE IC='" + IC + "'");
	db.~DBConnection();
}

void Account::UpdateVendorStatus(int stats) {
	DBConnection db;
	db.QueryStatement("UPDATE accounts SET isVendor ='" + to_string(stats) + "' WHERE IC='" + IC + "'");
	db.~DBConnection();
	if (stats == 0) IsVendor = false; else IsVendor = true;
}

bool Account::Login() {
	DBConnection db;
	db.QueryResult("SELECT * FROM accounts WHERE IC='" + IC + "' AND Password='" + Password + "'");
	db.~DBConnection();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			this->Name = db.res->getString("Name");
			this->PhoneNo = db.res->getString("PhoneNo");
			this->RegisterDate = db.res->getString("RegisterDate");
			if (db.res->getInt("IsVendor") == 0) IsVendor = false; else IsVendor = true;
		}
		return true;
	}
	else {
		return false;
	}
}

int Account::Register() {
	DBConnection db;
	try {
		db.QueryStatement("INSERT INTO accounts (IC , Name , Password , PhoneNo , RegisterDate ,isVendor) VALUES ('" + IC + "','" + Name +
			"','" + Password + "','" + PhoneNo + "','" + RegisterDate + "', 0 )");
		db.~DBConnection();
		return 0;
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() == 1062) {//duplicate PK entry: user already exist
			return 1;
		}
		else {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			return 3;
		}
	}
}


Account::Account(std::string IC) {
	DBConnection db;
	db.QueryResult("SELECT * FROM accounts WHERE IC='" + IC + "'");
	db.~DBConnection();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			this->IC = IC;
			this->Name = db.res->getString("Name");
			this->PhoneNo = db.res->getString("PhoneNo");
			this->RegisterDate = db.res->getString("RegisterDate");
			if (db.res->getInt("IsVendor") == 0) IsVendor = false; else IsVendor = true;
		}

	}
}


void Account::ReEvaluateVendorStatus() {
	DBConnection db;
	db.QueryResult("SELECT ShopId FROM shop WHERE OwnerIC='" + IC + "'");
	db.~DBConnection();
	if (db.res->rowsCount() == 0) {
		UpdateVendorStatus(0);
	}
}