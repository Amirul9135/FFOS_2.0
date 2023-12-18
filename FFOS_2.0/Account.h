#pragma once
#ifndef Account_H
#define Account_H

#include <string>  

class Account {
public:
	std::string IC;
	std::string Name;
	std::string Password;
	std::string PhoneNo;
	std::string RegisterDate;
	bool IsVendor;
	Account();
	Account(std::string IC);
	void UpdateName();
	void UpdatePassword();
	void UpdatePhoneNo();
	void UpdateVendorStatus(int stats);
	bool Login(); //return success or not
	int Register(); //0:success, 1:user already exist else other error
	void ReEvaluateVendorStatus();
};

#endif  