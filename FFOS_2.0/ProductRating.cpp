#include "DBConnection.h"
#include "ProductRating.h"
#include <mysql/jdbc.h> 
#include <string>  


using namespace sql;
using namespace std;

ProductRating::ProductRating() {
	ProductId = -1;
	Rating = -1;
}

ProductRating::ProductRating(int id, string Ic) {
	ProductId = -1;
	Rating = -1;
	DBConnection db;
	db.QueryResult("SELECT r.*, a.Name FROM productrating r,accounts a WHERE r.CustomerIc=a.IC AND r.CustomerIc='" + Ic + "' AND r.ProductId=" + to_string(id));
	db.~DBConnection();
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			ProductId = id;
			CustomerIC = Ic;
			Comment = db.res->getString("Comment");
			Rating = db.res->getInt("Rating");
			CustomerName = db.res->getString("Name");
		}
	}
}



void ProductRating::Update() {
	DBConnection db;
	db.QueryStatement("UPDATE productrating SET Comment='" + Comment + "',Rating=" + to_string(Rating) + " WHERE ProductId=" + to_string(ProductId) + " AND CustomerIc='" + CustomerIC + "'");
	db.~DBConnection();
	UpdateAverage();
}

void ProductRating::Insert() {
	DBConnection db;
	db.QueryStatement("INSERT INTO productrating(ProductId,CustomerIc,Comment,Rating) VALUES(" + to_string(ProductId) + ",'" + CustomerIC + "','" + Comment + "'," + to_string(Rating) + ")");
	db.~DBConnection();
	UpdateAverage();

}


void ProductRating::UpdateAverage() {
	double averageRate = 0.0;
	DBConnection db;
	db.QueryResult("SELECT Rating FROM productrating WHERE ProductId=" + to_string(ProductId)); 
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			averageRate += db.res->getInt("Rating");
		}
		averageRate = averageRate / db.res->rowsCount();
	}
	db.QueryStatement("UPDATE product SET Rating=" + to_string(averageRate) + " WHERE productId=" + to_string(ProductId));
	db.~DBConnection();
}