#pragma once
#ifndef Address_H
#define Address_H

#include <string>  

class Address {
public:
	std::string full;
	std::string Street;
	std::string Region;
	std::string PostCode;
	std::string City;
	std::string State;


	Address(std::string address);
	Address();
	std::string getPart1();
	std::string getPart2();
	std::string getFull();
	bool isValid();
};

#endif  