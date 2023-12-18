#include "Shop.h"
#include "DBConnection.h"
#include <mysql/jdbc.h> 
#include <string>  

using namespace sql;
using namespace std;


Shop::Shop() {
	ShopName = "";
	ContactNo = "";
	RegisterDate = "";
}


Shop::Shop(int id) {
	ShopId = id;
	DBConnection db;
	db.QueryResult("SELECT * FROM shop WHERE ShopId=" + to_string(id));
	db.~DBConnection();

	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			ShopId = id;
			OwnerIC = db.res->getString("OwnerIC"); 
			ShopName = db.res->getString("ShopName");
			Description = db.res->getString("Description");
			RegisterDate = db.res->getString("RegisterDate");
			ContactNo = db.res->getString("ContactNo");
			address = Address(db.res->getString("Address"));
		}
	}
}

void Shop::Register() {
	DBConnection db;
	db.QueryStatement("INSERT INTO shop(OwnerIC,ShopName,Description,Address,RegisterDate,ContactNo) VALUES ('" +
		OwnerIC + "','" + ShopName + "','" + Description + "','" + address.getFull() + "','" + RegisterDate + "','" + ContactNo + "')");
	db.~DBConnection();
}

vector<Shop> Shop::FindShop(string keyName, string keyAddress, string IC) {
	DBConnection db;
	vector<Shop> FoundShops;
	Shop tmpShop;
	string strSql = "SELECT * FROM shop WHERE ";
	if (!IC.empty()) {
		strSql += "OwnerIC='" + IC + "' AND ";
	}
	strSql += "ShopName LIKE '%" + keyName + "%' AND Address LIKE '%" + keyAddress + "%'";
	db.QueryResult(strSql); //already collate _ci in db for case incensitive
	//cout << "SELECT * FROM shop WHERE ShopName LIKE '%" + keyName + "%' AND Address LIKE '%" + keyAddress + "%'"; //for testing 
	db.~DBConnection();
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			tmpShop.ShopId = db.res->getInt("ShopId");
			tmpShop.OwnerIC = db.res->getString("OwnerIC");
			tmpShop.ShopName = db.res->getString("ShopName");
			tmpShop.Description = db.res->getString("Description");
			tmpShop.ContactNo = db.res->getString("ContactNo");
			tmpShop.address = Address(db.res->getString("Address"));
			tmpShop.RegisterDate = db.res->getString("RegisterDate");
			FoundShops.push_back(tmpShop);
		}
	}
	return FoundShops;
}

vector<Shop> Shop::FindShop() {
	return FindShop("", "", "");
}

vector<Shop> Shop::FindShop(string keyName, string keyAddress) {
	return FindShop(keyName, keyAddress, "");
}

void Shop::Update() {
	DBConnection db;
	db.QueryStatement("UPDATE shop SET ShopName ='" + ShopName + "',Description ='" + Description + "', ContactNo ='" + ContactNo + "', Address = '"
		+ address.getFull() + "' WHERE ShopId='" + to_string(ShopId) + "'");
	db.~DBConnection();
}


void Shop::Delete() {
	DBConnection db;
	db.QueryStatement("DELETE FROM shop WHERE ShopId=" + to_string(ShopId));
	db.~DBConnection();
}


