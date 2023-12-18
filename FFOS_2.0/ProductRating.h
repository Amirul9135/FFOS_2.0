#pragma once

#ifndef ProductRating_H
#define ProductRating_H
#include <string>

class ProductRating {
public:
	int ProductId;
	std::string CustomerIC;
	std::string Comment;
	int Rating;
	//add
	std::string CustomerName;

	ProductRating();
	ProductRating(int id, std::string ic);
	void Update();
	void Insert();
private:
	void UpdateAverage();
};

#endif  