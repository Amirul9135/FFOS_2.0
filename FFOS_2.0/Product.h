#pragma once
#ifndef Product_H
#define Product_H

#include <string>  
#include <vector>    
#include "ProductRating.h"

class Product {
public:
	int ProductId;
	int ShopId;
	std::string Name;
	std::string Description;
	float Price;
	float Rating;
	int Quantity;//used by order class
	std::string ShopName;//used on get all


	Product();
	Product(int id);
	void Register();
	void Update();
	void Delete();
	void RefreshRating();
	bool isEligibleRate(std::string userIc);
	std::vector<ProductRating> getAllRating();

	std::vector<Product> Find();
	std::vector<Product> SortByPrice(bool);
	std::vector<Product> SortByRating(bool);


	std::vector<Product> getAllProduct();
	std::vector<Product> getAllProductSortByPrice(bool);
	std::vector<Product> getAllProcuctSortByRating(bool);

	friend bool operator==(const Product& one, const Product& two);

private:
	std::vector<Product> Search(int, bool);
	std::vector<Product> getAll(int,bool);


};
#endif  