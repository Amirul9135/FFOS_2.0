#pragma once
#include "Orders.h"
#include "Product.h"
#include "DBConnection.h"
#include <mysql/jdbc.h> 
#include <string>  


using namespace sql;
using namespace std;

Orders::Orders() {
	TotalPrice = 0.0;
	PaymentRecieved = 0.0;
	Status = -1;//Status: -1 newly created not yet in db, { "New", "Accepted" , "Ready" ,"Canceled", "Refused" , "Failed" , "Completed","Pending Payment" }
	ShopId = -1;
}

Orders::Orders(Account* customer, Shop* shop) {
	CustomerIC = customer->IC;
	ShopId = shop->ShopId;
	Status = -1;
	TotalPrice = 0.0;
	PaymentRecieved = 0.0;
	ShopName = shop->ShopName;
	CustName = customer->Name;
}

Orders::Orders(int id) {
	if (id >= 0) {
		DBConnection db;
		db.QueryResult("SELECT * FROM orders WHERE OrderId ='" + to_string(id) + "'");
		if (db.res->rowsCount() > 0) {
			while (db.res->next()) {
				OrderId = db.res->getInt("OrderId");
				CustomerIC = db.res->getString("CustomerIC");
				ShopId = db.res->getInt("ShopId");
				Comment = db.res->getString("Comment");
				TotalPrice = db.res->getDouble("TotalPrice");
				PaymentRecieved = db.res->getDouble("PaymentRecieved");
				OrderDate = db.res->getString("OrderDate");
				CompleteDate = db.res->getString("CompleteDate");
				Status = db.res->getInt("Status");
			}
			db.QueryResult("SELECT Name,PhoneNo FROM accounts WHERE IC='" + CustomerIC + "'");
			if (db.res->rowsCount() == 1) {
				while (db.res->next()) {
					CustName = db.res->getString("Name");
					CustContactNo = db.res->getString("PhoneNo");
				}
			}
			db.QueryResult("SELECT ShopName,ContactNo FROM shop WHERE ShopId=" + to_string(ShopId));
			if (db.res->rowsCount() == 1) {
				while (db.res->next()) {
					ShopName = db.res->getString("ShopName");
					ShopContactNo = db.res->getString("ContactNo");
				}
			}
			db.~DBConnection();

			DBConnection db2;
			db2.QueryResult("SELECT o.*,p.Name FROM orderitem o,product p WHERE o.ProductId=p.ProductId AND OrderId='" + to_string(OrderId) + "'");
			db2.~DBConnection();
			Product tmpProd;
			if (db2.res->rowsCount() > 0) {
				while (db2.res->next()) {
					tmpProd.ProductId = db2.res->getInt("ProductId");
					tmpProd.Name = db2.res->getString("Name");
					tmpProd.Price = db2.res->getDouble("Price");
					tmpProd.Quantity = db2.res->getInt("quantity");
					this->Orderitems.push_back(tmpProd);
				}
			}
		}
	}
}

void Orders::InsertNew() {
	if (Status == -1) {
		DBConnection db;
		db.QueryStatement("INSERT INTO orders(CustomerIC,ShopId,TotalPrice,OrderDate,Status) VALUES ('" +
			CustomerIC + "','" + to_string(ShopId) + "','" + to_string(TotalPrice) + "','" + OrderDate + "','0');");
		db.QueryResult("SELECT LAST_INSERT_ID();");
		db.~DBConnection();
		if (db.res->rowsCount() > 0) {
			while (db.res->next()) {
				OrderId = db.res->getInt64("LAST_INSERT_ID()");
				InsertOrderItems();
			}
		}

	}
}


void Orders::InsertOrderItems() {
	if (!Orderitems.empty()) {
		DBConnection db;
		for (Product tmpProd : Orderitems) {
			db.QueryStatement("INSERT INTO orderitem(OrderId,ProductId,Price,OrderDate,quantity) VALUES ('" +
				to_string(OrderId) + "','" + to_string(tmpProd.ProductId) + "','" + to_string(tmpProd.Price) + "','" + OrderDate + "','" + to_string(tmpProd.Quantity) + "');");
		}
		db.~DBConnection();
	}
}

void Orders::Update() {

}
void Orders::Delete() {

}
void Orders::addProduct(Product nProduct) {
	vector<Product>::iterator I = find(Orderitems.begin(), Orderitems.end(), nProduct);
	if (I != Orderitems.end()) {
		Orderitems[distance(Orderitems.begin(), I)].Quantity++;
	}
	else {
		nProduct.Quantity = 1;
		Orderitems.push_back(nProduct);
	}
	calcTotalPrice();
}
void Orders::removeProduct(Product nProduct) {
	if (!Orderitems.empty()) {
		vector<Product>::iterator I = find(Orderitems.begin(), Orderitems.end(), nProduct);
		if (I != Orderitems.end()) {
			int i = distance(Orderitems.begin(), I);
			if (Orderitems[i].Quantity > 1) {
				Orderitems[i].Quantity--;
			}
			else if (Orderitems[i].Quantity <= 1) {
				Orderitems.erase(I);
			}
			calcTotalPrice();
		}
	}
}


vector<Orders> Orders::getOrderSortByDate(bool ASC, int stats) {
	return getOrderList(0, ASC, stats);
}


vector<Orders> Orders::getOrderSortByTotalPrice(bool ASC, int stats) {
	return getOrderList(2, ASC, stats);
}


void Orders::calcTotalPrice() {
	if (!Orderitems.empty()) {
		TotalPrice = 0;
		for (int i = 0; i < Orderitems.size(); i++) {
			TotalPrice = TotalPrice + Orderitems[i].Price * Orderitems[i].Quantity;
		}
	}
}




vector<Orders> Orders::getOrderList(int sort, bool ASC, int sstatus) { //0:by date, 1:by status, 2:by price
	if ((!CustomerIC.empty() && CustomerIC.at(0) != '\0') || ShopId >= 0) {
		vector<Orders> resOrders;
		Orders tmp;
		DBConnection db;
		string strSql = "SELECT s.ShopName,s.ContactNo,o.OrderId,o.CustomerIC,o.ShopId,o.TotalPrice,o.OrderDate,o.Status,a.Name,a.PhoneNo FROM orders o,shop s,accounts a WHERE o.ShopId=s.ShopId AND o.CustomerIC=a.IC"; //not *.. no ned comment in list

		if ((!CustomerIC.empty() && CustomerIC.at(0) != '\0')) {
			strSql += " AND o.CustomerIC LIKE '%" + CustomerIC + "%' ";
		}
		if (ShopId >= 0) {
			strSql += " AND o.ShopId='" + to_string(ShopId) + "' ";
		}
		if (sstatus != -1) {
			strSql += " AND o.Status=" + to_string(sstatus) + " ";
		}
		strSql += "AND s.ShopName LIKE '%" + ShopName + "%' ";
		if (sort == 0) {
			strSql += " ORDER BY o.OrderDate";
		}
		else if (sort == 1) {
			strSql += " ORDER BY o.Status";
		}
		else if (sort == 2) {
			strSql += " ORDER BY o.TotalPrice";
		}
		if (ASC) {
			strSql += " ASC";
		}
		else {
			strSql += " DESC";
		}

		db.QueryResult(strSql);
		db.~DBConnection();

		if (db.res->rowsCount() > 0) {
			while (db.res->next()) {
				tmp.OrderId = db.res->getInt("OrderId");
				tmp.CustomerIC = db.res->getString("CustomerIC");
				tmp.ShopId = db.res->getInt("ShopId");
				tmp.TotalPrice = db.res->getDouble("TotalPrice");
				tmp.OrderDate = db.res->getString("OrderDate");
				tmp.Status = db.res->getInt("Status");
				tmp.ShopName = db.res->getString("ShopName");
				tmp.ShopContactNo = db.res->getString("ContactNo");
				tmp.CustName = db.res->getString("Name");
				tmp.CustContactNo = db.res->getString("PhoneNo");
				resOrders.push_back(tmp);
			}
		}
		return resOrders;
	}
}

void Orders::updateStatus(int stats) {
	DBConnection db;
	db.QueryStatement("UPDATE orders SET Status =" + to_string(stats) + " WHERE OrderId=" + to_string(OrderId));
	db.~DBConnection();
}


void Orders::CompleteOrder(float payment, string completeDate) {
	DBConnection db;
	db.QueryStatement("UPDATE orders SET Status =6, PaymentRecieved =" + to_string(payment) + ", CompleteDate='" + completeDate + "' WHERE OrderId=" + to_string(OrderId));
	db.~DBConnection();
}


void Orders::UpdatePendingPayment(float payment) {
	DBConnection db;
	db.QueryStatement("UPDATE orders SET Status =7, PaymentRecieved =" + to_string(payment) + " WHERE OrderId=" + to_string(OrderId));
	db.~DBConnection();
}



void Orders::FailByVendorComment(string comment) {
	string fullComment;
	DBConnection db;
	db.QueryResult("SELECT Comment FROM orders WHERE OrderId=" + to_string(OrderId));
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			fullComment = db.res->getString("Comment");
		}
	}
	fullComment = fullComment + "<FV>" + comment + "<FV>";
	db.QueryStatement("UPDATE orders SET Status=5 , Comment='" + fullComment + "' WHERE OrderId=" + to_string(OrderId));
	db.~DBConnection();
}