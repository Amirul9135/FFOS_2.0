#pragma once
#ifndef Shop_H
#define Shop_H

#include <string>  
#include <vector>   
#include "Address.h"

class Shop {
public:
	int ShopId;
	std::string OwnerIC;
	std::string ShopName;
	std::string Description;
	std::string ContactNo;
	std::string RegisterDate;
	Address address;

	Shop();
	Shop(int ShopId);
	void Register();
	std::vector<Shop> FindShop(std::string keyName, std::string keyAddress);
	std::vector<Shop> FindShop();
	std::vector<Shop> FindShop(std::string keyName, std::string keyAddress, std::string IC);
	void Update();
	void Delete();
};

#endif  
