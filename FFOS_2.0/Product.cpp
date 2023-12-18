#include "Product.h"
#include "DBConnection.h"
#include <mysql/jdbc.h> 
#include <string>  

using namespace sql;
using namespace std;


Product::Product() {
	Name = "";
	Description = "";
	Price = 0.0;
	Rating = 0.0;
	ShopId = -1;
	Quantity = 0;
}

Product::Product(int id) {
	DBConnection db;
	db.QueryResult("SELECT * FROM product WHERE ProductId=" + to_string(id));
	db.~DBConnection();

	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			ProductId = id;
			ShopId = db.res->getInt("ShopId");
			Name = db.res->getString("Name");
			Description = db.res->getString("Description");
			Price = db.res->getDouble("Price");
			Rating = db.res->getDouble("Rating");
		}
	}
}

void Product::Register() {
	DBConnection db;
	db.QueryStatement("INSERT INTO product(ShopId,Name,Description,Price,Rating) VALUES ('" +
		to_string(ShopId) + "','" + Name + "','" + Description + "','" + to_string(Price) + "','" + to_string(Rating) + "')");
	db.~DBConnection();

}

void Product::Update() {
	DBConnection db;
	db.QueryStatement("UPDATE product SET Name ='" + Name + "', Description ='" + Description + "', Price = '" + to_string(Price)
		+ "', Rating='" + to_string(Rating) + "' WHERE ProductId = '" + to_string(ProductId) + "'");
	db.~DBConnection();
}
void Product::Delete() {
	DBConnection db;
	db.QueryStatement("DELETE FROM product WHERE ProductId=" + to_string(ProductId));
	db.~DBConnection();
}

void Product::RefreshRating() {
	DBConnection db;
	db.QueryResult("SELECT Rating FROM product WHERE ProductId=" + to_string(ProductId));
	db.~DBConnection();
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			Rating = db.res->getDouble("Rating");
		}
	}
}


vector<Product> Product::Search(int sort, bool asc) { //sort 0 unsort, 1 price, 3 rating ;; asc=ascending
	vector<Product> resProduct;
	Product tmpProduct;
	DBConnection db;
	string strSql = "SELECT * FROM product WHERE Name LIKE '%" + Name + "%'";
	if (ShopId != -1) {
		strSql += " AND ShopId=" + to_string(ShopId);
	}
	if (sort != 0)
		strSql += " ORDER BY ";

	if (sort == 1)
		strSql += "Price";
	else if (sort == 3)
		strSql += "Rating";

	if (sort != 0) {
		if (asc)
			strSql += " ASC";
		else
			strSql += " DESC";
	}


	db.QueryResult(strSql);
	db.~DBConnection();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			tmpProduct.ProductId = db.res->getInt("ProductId");
			tmpProduct.ShopId = db.res->getInt("ShopId");
			tmpProduct.Name = db.res->getString("Name");
			tmpProduct.Description = db.res->getString("Description");
			tmpProduct.Price = db.res->getDouble("Price");
			tmpProduct.Rating = db.res->getDouble("Rating");
			resProduct.push_back(tmpProduct);
		}
	}


	return resProduct;
}


vector<Product> Product::SortByPrice(bool asc) {
	return Search(1, asc);
}


vector<Product> Product::SortByRating(bool asc) {
	return Search(3, asc);
}

vector<Product> Product::Find() {
	return Search(0, false);
}


bool Product::isEligibleRate(string userIc) {
	DBConnection db;
	db.QueryResult("SELECT o.OrderId FROM orders o,orderitem i WHERE o.OrderId=i.OrderId AND i.ProductId=" + to_string(ProductId) + " AND o.CustomerIC='" + userIc + "' AND o.Status=6");
	db.~DBConnection();
	if (db.res->rowsCount() > 0)
		return true;
	else
		return false;
}


bool operator==(const Product& one, const Product& two) {
	return one.ProductId == two.ProductId;
}


vector<ProductRating> Product::getAllRating() {
	vector<ProductRating> allRating; 
	DBConnection db;
	db.QueryResult("SELECT r.*,a.Name FROM productrating r,accounts a WHERE r.CustomerIc=a.IC AND r.ProductId=" + to_string(ProductId));
	db.~DBConnection();
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			ProductRating tmp;
			tmp.ProductId = ProductId;
			tmp.CustomerIC = db.res->getString("CustomerIc");
			tmp.CustomerName = db.res->getString("Name");
			tmp.Comment = db.res->getString("Comment");
			tmp.Rating = db.res->getInt("Rating");
			allRating.push_back(tmp);
		}
	}
	return allRating;
}


vector<Product> Product::getAllProduct() {
	return getAll(0, false);
}
vector<Product> Product::getAllProductSortByPrice(bool asc) {
	return getAll(1, asc);
}
vector<Product> Product::getAllProcuctSortByRating(bool asc) {
	return getAll(2, asc);
}

vector<Product> Product::getAll(int sort, bool asc) {
	vector<Product> allProd;
	DBConnection db;
	string strSql = "SELECT p.*,s.ShopName FROM product p,shop s WHERE p.ShopId=s.ShopId AND Name LIKE '%" + Name + "%' ";
	if (sort != 0) {
		strSql += " ORDER BY ";
		if (sort == 1)
			strSql += "Price";
		else if (sort == 2)
			strSql += "Rating";

		if(asc)
			strSql += " ASC";
		else
			strSql += " DESC";
	}
	db.QueryResult(strSql);
	db.~DBConnection();

	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			Product tmpProduct;
			tmpProduct.ProductId = db.res->getInt("ProductId");
			tmpProduct.ShopId = db.res->getInt("ShopId");
			tmpProduct.Name = db.res->getString("Name");
			tmpProduct.Description = db.res->getString("Description");
			tmpProduct.Price = db.res->getDouble("Price");
			tmpProduct.Rating = db.res->getDouble("Rating");
			tmpProduct.ShopName = db.res->getString("ShopName");
			allProd.push_back(tmpProduct);
		}
	}
	return allProd;

}