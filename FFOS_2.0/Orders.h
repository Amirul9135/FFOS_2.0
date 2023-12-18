#pragma once
#ifndef Orders_H
#define Orders_H

#include <string>  
#include <vector>    
#include "Product.h"
#include "Shop.h"
#include "Account.h"

class Orders {
public:
	int OrderId;
	std::string CustomerIC;
	int ShopId;
	std::string Comment;
	std::string OrderDate;
	std::string CompleteDate;
	int Status;
	float TotalPrice;
	float PaymentRecieved;
	std::vector<Product> Orderitems;
	//program only value
	std::string ShopName;
	std::string CustName;
	std::string ShopContactNo;
	std::string CustContactNo;

	Orders();
	Orders(Account* customer, Shop* shop);
	Orders(int id);
	void InsertNew();
	void Update();
	void Delete();
	void addProduct(Product);
	void removeProduct(Product);
	std::vector<Orders> getOrderSortByStatus(bool ASC, int);
	std::vector<Orders> getOrderSortByDate(bool ASC, int);
	std::vector<Orders> getOrderSortByTotalPrice(bool ASC, int);
	void updateStatus(int);
	void CompleteOrder(float, std::string);
	void UpdatePendingPayment(float);
	void FailByVendorComment(std::string comment);
private:
	void calcTotalPrice();
	void InsertOrderItems();
	std::vector<Orders> getOrderList(int, bool, int); // set key directly to the attribute 


};
#endif  
