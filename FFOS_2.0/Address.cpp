#include "Address.h"

#include <string>  
using namespace std;

Address::Address() {

}

Address::Address(string address) {
	Street = address.substr(0, address.find_first_of(','));
	address = address.substr(address.find_first_of(',') + 2);
	Region = address.substr(0, address.find_first_of(','));
	address = address.substr(address.find_first_of(',') + 2);
	PostCode = address.substr(0, address.find_first_of(','));
	address = address.substr(address.find_first_of(',') + 2);
	City = address.substr(0, address.find_first_of(','));
	address = address.substr(address.find_first_of(',') + 2);
	State = address.substr(0, address.find_first_of(','));
}


string Address::getPart1() {
	return  Street + ", " + Region;
}

string Address::getPart2() {
	return PostCode + ", " + City + ", " + State;;
}

string Address::getFull() {
	return Street + ", " + Region + ", " + PostCode + ", " + City + ", " + State;
}

bool Address::isValid() {
	if (Street.empty() || Street.at(0) == '\0') return false;
	if (Region.empty() || Region.at(0) == '\0') return false;
	if (PostCode.empty() || PostCode.at(0) == '\0') return false;
	if (City.empty() || City.at(0) == '\0') return false;
	if (State.empty() || State.at(0) == '\0') return false;

	return true;

}