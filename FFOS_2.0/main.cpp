#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <stdio.h>
#include <cstdio>
#include <algorithm>
#include <ctime> 
#include <cmath>
#include <format>   
//custom class 
#include "Account.h" 
#include "Shop.h"
#include "Address.h"
#include "Product.h"
#include "Orders.h"
#include "SaleAnalysis.h"    
#include "ProductRating.h"
//namespace
using namespace std;

//main modules 
void Register();
void Login();
void MainMenu(Account* user);
void Profile(Account* user);
void RegisterShop(Account* user, Shop* shop = NULL);
void FindShop(Account* user, bool vendorMode = false, int dest = 0);
void VendorMenu(Account* user);
bool ShopPage(Account* user, Shop* shop , Product* carryOverProduct = NULL);
void FindAllProduct(Account* user);
bool ProductDetails(Account* user, Shop* shop = NULL, Product* product = NULL, bool viewOnly = false, bool fromOrder = false);
bool OrderDetails(Account*, Orders* = NULL);
void ManageOrder(Account* user = NULL, Shop* myShop = NULL);
void SaleAnalysisMenu(Account* user, Shop* myShop);
void RateProduct(Account* user, Product* productToRate);
void ViewAllRating(Account* user, Product* product);
//sub modules
int parseDateToYear(string);
int parseDateToMonth(string);
void changePassword(string* curPass);
void insertAddress(Address* address);
string ParseComment(string comment);
void displayMenuGuide(bool editable = false);
void displayInsertGuide(int, int);
//functions
int LinerSearchProduct(vector<Product> * p, Product * key);
void ChangeScreen(string title);
void MoveCursor(int x, int y);
string getchLine(int max, int mode, string* curString = NULL, int dPlace = 0);
void set_cursor(bool visible);
COORD getCursorPos();
string strPlaceHolder(string text, char placeHolder);
string strDateTimeNow();
COORD getConsoleSize();
void setConsoleSize(int width, int height);
void printLine(char c);
string strTrimSpaceEnd(string str);
string strTrimSpaceStart(string str);
void strTrimSpace(string* str);
void generateVerticalSpace(int max);
float roundByDecimalPlace(float num, int decimalPlace);
float myStrToFloat(string* num);
//=================================================

int main() {
	setConsoleSize(1280, 1000);
	ChangeScreen(" ");
	int intOpt = 0;  
	char  option = ' ';  

	while (option != '\0') {
		system("cls");
		set_cursor(false);
		cout << "Welcome to Frozen Food Ordering System" << endl << endl;

		if (intOpt == 0) {
			cout << "\u001b[33m>";
		}
		cout << ">Login\u001b[0m" << endl;
		if (intOpt == 1) {
			cout << "\u001b[33m>";
		}
		cout << ">Register\u001b[0m" << endl;
		if (intOpt == 2) {
			cout << "\u001b[33m>";
		}
		cout << ">Exit\u001b[0m" << endl;
		cout << endl << "====NOTE====";
		cout << endl << "---Press \u001b[33mUp/Down arrow\u001b[0m key to select an option in the menus" << endl << "---Press \u001b[33mEnter\u001b[0m after selecting an option to proceed"
			<< endl << "---Press \u001b[33mEscape(ESC)\u001b[0m to navigate to previous menu/exit";
		option = _getch();
		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = 2;
				}
				break;
			case 80://down arrow    
				if (intOpt < 2) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				switch (intOpt) {
				case 0:
					Login();
					break;
				case 1:
					Register();
					break;
				case 2:
					exit(0);
					break;
				}
			}
		}
	}
	return 0;
}

void MainMenu(Account* user) {
	char option = ' ';
	int intOpt = 0;
	int maxOption;
	ChangeScreen("MainMenu");
	do {

		if (user->IsVendor)
			maxOption = 5;
		else
			maxOption = 4;
		system("cls");
		set_cursor(false);
		cout << "WELCOME TO FROZEN FOOD ORDERING SYSTEM " << endl << "User : " << user->Name << endl;
		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">Profile\u001b[0m" << endl;
		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Find Products\u001b[0m" << endl;
		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Find Shops\u001b[0m" << endl;
		if (intOpt == 3)
			cout << "\u001b[33m>";
		cout << ">My orders\u001b[0m" << endl;
		if (intOpt == 4)
			cout << "\u001b[33m>";
		cout << ">Register Shop\u001b[0m" << endl;
		if (user->IsVendor) {
			if (intOpt == 5)
				cout << "\u001b[33m>";
			cout << ">Vendor Menu\u001b[0m" << endl;
		}
		displayMenuGuide();
		option = _getch();

		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = maxOption;
				}
				break;
			case 80://down arrow    
				if (intOpt < maxOption) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				switch (intOpt) {
				case 0:
					Profile(user);
					break;
				case 1:
					FindAllProduct(user);
					break;
				case 2: //find shop
					FindShop(user);
					break;
				case 3:
					ManageOrder(user);
					break;
				case 4:
					RegisterShop(user);
					break;
				case 5:
					if (user->IsVendor) {
						VendorMenu(user);
					}
					break;
				}
			}
			else if (option == 27) {
				main();
			}
		}
	} while (option != '\0');
}

void Profile(Account* user) {
	ChangeScreen("Profile");
	char option = ' ';
	int intOpt = 0;
	bool error = false;
	bool changes = false;
	string name = user->Name;
	string password = user->Password;
	string phone = user->PhoneNo;
	do {
		set_cursor(false);
		system("cls");
		cout << "User's Profile";
		cout << endl << "Identification (IC) No.: " << user->IC << endl;
		cout << "Date Registered : " << user->RegisterDate;
		printLine('_');
		cout << endl;
		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">Name : " << name << "\u001b[0m" << endl;
		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Password : " << strPlaceHolder(password, '*') << "\u001b[0m" << endl;
		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Phone Number : " << phone << "\u001b[0m" << endl;
		if (intOpt == 3)
			cout << "\u001b[33m>";
		cout << ">Save Changes" << "\u001b[0m";

		printLine('_');
		error = false;
		if (name.size() == 0) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Please fill in your name \u001b[0m";
			error = true;
		}
		if (password.size() < 6) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid password, Must contain 6 to 12 character \u001b[0m";
			error = true;
		}
		if (phone.size() < 10) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid Phone Number \u001b[0m";
			error = true;
		}
		if (!error && (name != user->Name || password != user->Password || phone != user->PhoneNo)) {
			cout << "\u001b[42;1m\u001b[37m\n  Please confirm your informations and press enter to apply changes  \u001b[0m";
			changes = true;
		}
		else {
			changes = false;
		}

		displayMenuGuide(true);
		option = _getch();

		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = 3;
				}
				break;
			case 80://down arrow    
				if (intOpt < 3) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				switch (intOpt) {
				case 0:
					displayInsertGuide(100, 2);
					MoveCursor(9, 4);
					set_cursor(true);
					name = getchLine(100, 2, &name);
					break;
				case 1:
					password = user->Password;
					changePassword(&password);
					break;
				case 2:
					displayInsertGuide(11, 3);
					MoveCursor(17, 6);
					set_cursor(true);
					phone = getchLine(11, 3, &phone);
					break;
				case 3:
					if (changes) {
						if (name != user->Name) {
							user->Name = name;
							user->UpdateName();
						}
						if (password != user->Password) {
							user->Password = password;
							user->UpdatePassword();
						}
						if (phone != user->PhoneNo) {
							user->PhoneNo = phone;
							user->UpdatePhoneNo();
						}

					}
					break;
				}
			}
			else if (option == 27) {
				return;
			}
		}
	} while (option != '\0');

}

void changePassword(string* curPass) {
	string pass;
	char option = ' ';
	do {
		system("cls");
		set_cursor(false);
		cout << "Press Enter  to Insert OR Press Escape(ESC) to go back \nCurrent Password : ";
		option = _getch();
		if (option == 27) {
			return;
		}
		else if (option == 13) {
			pass = "";
			set_cursor(true);
			pass = getchLine(12, 1, &pass);
			if (pass == *curPass) {
				cout << "\nEnter New password : ";
				string npass;
				npass = getchLine(12, 1, &npass);
				if (npass.size() < 6) {
					cout << "\u001b[41;1m\u001b[37m\n Error : Invalid password, Must contain 6 to 12 character \u001b[0m";
					_getch();
				}
				else {
					*curPass = npass;
					cout << "\nPassword changed, press any key to go back and choose save changes";
					_getch();
					return;
				}
			}
			else {
				cout << "\u001b[41;1m\u001b[37m\n Error : Incorrect password, Please try again \u001b[0m";
				_getch();
			}
		}
	} while (option != '\0');
}

void FindShop(Account* user, bool vendorMode, int dest) {//dest 1 manage order 2 sale analysis
	ChangeScreen("Find Shop");
	set_cursor(false);
	fflush(stdin);
	Shop findShop;
	vector<Shop> shopList;
	char c = ' ';
	int maxCol = 0;
	bool forwardOption = false;
	bool insName = false;
	bool insAddress = false;
	string keyName, keyAddress;
	bool changes = true; // to minimize requery, if not even if user press invalid char reloop and requeery happen
	int Selected = -1;
	do {
		system("cls");
		set_cursor(false);
		if (changes) {
			if (vendorMode) {
				shopList = findShop.FindShop(keyName, keyAddress, user->IC);
			}
			else {
				shopList = findShop.FindShop(keyName, keyAddress);
			}
			Selected = 0;
		}
		if (dest == 1)
			cout << "Select a shop to manage its order" << endl;

		cout << "\u001b[33m\n\033[4mFilter/Search\n\033[0m";
		if (insName) { cout << "\u001b[33m>"; }
		cout << "Shop Name : " + keyName + "\u001b[0m\n";
		if (insAddress) { cout << "\u001b[33m>"; }
		cout << "Address : " + keyAddress + "\u001b[0m";
		cout << endl << "[ Found :  " << shopList.size() << " ] ";
		printLine('=');
		if (!shopList.empty()) {
			maxCol = getConsoleSize().X;
			for (int i = 0; i < shopList.size(); i++) {
				if (i == Selected || i - Selected == 1 || Selected - i == 1 || (Selected == 0 && i - Selected == 2) || (Selected == shopList.size() - 1 && Selected - i == 2)) { // ensure only 3 max displayed
					if (i == Selected) {
						cout << "\u001b[36m";
					}
					printLine('_');
					cout << "\nShop Name\t: " + shopList[i].ShopName << endl;
					if (shopList[i].Description.size() > maxCol) {
						cout << "-" << shopList[i].Description.substr(0, maxCol - 5) << "..." << endl;
					}
					else {
						cout << "-" << shopList[i].Description << endl;
					}

					cout << "Contact No.\t: " + shopList[i].ContactNo << endl;
					cout << "Address\t: " + shopList[i].address.getPart1() << endl;
					cout << "\t  " + shopList[i].address.getPart2();
					printLine('_');
					if (i == Selected) {
						cout << "\u001b[0m";
					}
				}

			}
		}

		printLine('=');

		cout << endl << "\n====GUIDE====" << endl << "---Press \u001b[33mUp/Down arrow\u001b[0m key to Select a shop" << endl
			<< "---Press \u001b[33mEnter\u001b[0m to go to selected shop page" << endl;
		if (!insAddress && !insName)
			cout << "---Press \u001b[33m1\u001b[0m to insert/edit Shop Name filter" << endl << "---Press \u001b[33m2\u001b[0m to insert/edit Address filter"
			<< endl << "---Press \u001b[33mEscape(ESC)\u001b[0m to go back";
		if (insName || insAddress)
			cout << "---Press \u001b[33mBackspace\u001b[0m to delete a character in the filter" << endl << "---Press \u001b[33mEscape(ESC)\u001b[0m to stop inserting/edit filter" << endl;


		if (!forwardOption) {
			c = _getch();
		}
		else {
			forwardOption = false;
		}


		if (int(c) == -32) { //special char eg:arrow
			changes = false;
			c = _getch();
			switch (c) {
			case 72: //Up Arrow   
				if (Selected > 0) {
					Selected--;
				}
				break;
			case 80://down arrow    
				if (Selected < shopList.size() - 1) {
					Selected++;
				}
				break;
			default:
				break;
			}
		}
		else {
			switch (c) {
			case 72: //Up Arrow    //2nd catch for forwarded option
				if (Selected > 0) {
					Selected--;
				}
				break;
			case 80://down arrow    
				if (Selected < shopList.size() - 1) {
					Selected++;
				}
				break;
			case '\r':
				if (insName) insName = false;
				if (insAddress) insAddress = false;
				changes = false;
				if (!insName && !insAddress) {
					if (dest == 1) {
						ManageOrder(user, &shopList[Selected]);
					}
					else if (dest == 2) {
						SaleAnalysisMenu(user, &shopList[Selected]);
					}
					else {
						changes = ShopPage(user, &shopList[Selected]);
					}

				}
				break;
			case 27:
				if (insAddress || insName) {
					insAddress = false;
					insName = false;
				}
				else {
					return;
				}
				changes = false;
				break;
			case '1': //Shop name insert
				changes = false;
				if (!(insAddress || insName)) {
					insName = true;
					break;
				}
			case '2': //address insert
				changes = false;
				if (!(insAddress || insName)) {
					insAddress = true;
					break;
				}
			default:
				if (insName || insAddress) {
					if ((int(c) >= 65 && int(c) <= 90) || (int(c) >= 97 && int(c) <= 122) || int(c) == 32 || (int(c) >= 48 && int(c) <= 57)) { //char and number and space only
						if (insName) {
							keyName += c;
						}
						else if (insAddress) {
							keyAddress += c;
						}
						changes = true;
					}
					else if (c == '\b') {
						if (insName) {
							if (keyName.size() > 0) {
								keyName.resize(keyName.size() - 1);
								changes = true;
							}
							else {
								changes = false;
							}
						}
						if (insAddress) {
							if (keyAddress.size() > 0) {
								keyAddress.resize(keyAddress.size() - 1);
								changes = true;
							}
							else {
								changes = false;
							}

						}
					}
					else {
						changes = false; //if inserting but character is invalid aka not within range means no change, no need to re query
					}
				}
				else {
					changes = false; //if not inserting aka still in find shop main window but user insert unnecessary char, no change so no ned to requery at top
					do {
						c = _getch();
						if (c == '1' || c == '2' || c == '\r' || c == 27) {
							break;
						}
						if (int(c) == -32) {
							c = _getch();
							if (c == 72 || c == 80) {
								break;
							}
						}
					} while (c != '\0');
					forwardOption = true;
				}
				break;
			}
		}


	} while (c != '\0');

}

void RegisterShop(Account* user, Shop* shop) {
	ChangeScreen("Shop Registration");
	char option = ' ';
	int intOpt = 0;
	Shop nShop;
	bool Error[4][2] = { false };//1st row for error 2nd row to make sure if user has try inputting 
	if (shop != NULL) {
		ChangeScreen("Edit Shop");
		nShop = *shop;
		Error[0][1] = true;
		Error[1][1] = true;
		Error[2][1] = true;
		Error[3][1] = true;
	}
	do {
		set_cursor(false);
		system("cls");
		cout << "Please fill in the following details of your shop" << endl << endl;

		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">Shop Name   : " << nShop.ShopName << "\u001b[0m" << endl;
		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Contact No. : " << nShop.ContactNo << "\u001b[0m" << endl;
		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Address"; if (intOpt != 2) cout << " "; cout << "   : " << nShop.address.getPart1();
		cout << endl << "  Town/City : " << nShop.address.PostCode << ", " << nShop.address.City;
		cout << endl << "  State     : " << nShop.address.State << "\u001b[0m" << endl;
		if (intOpt == 3)
			cout << "\u001b[33m>";
		cout << ">Description : " << nShop.Description << "\u001b[0m" << endl << endl;
		if (intOpt == 4)
			cout << "\u001b[33m>";
		cout << ">Confirm & Proceed\u001b[0m" << endl;


		if (Error[0][0]) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Please fill in shop name \u001b[0m";
		}
		if (Error[1][0]) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid Contact Number \u001b[0m";
		}
		if (Error[2][0]) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid Address \u001b[0m";
		}
		if (Error[3][0]) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Please enter a short description of your shop \u001b[0m";
		}

		if ((!Error[0][0] && Error[0][1]) && (!Error[1][0] && Error[1][1]) && (!Error[2][0] && Error[2][1]) && (!Error[3][0] && Error[3][1])) {
			cout << "\u001b[42;1m\u001b[37m\n  Please confirm your informations and press enter to proceed  \u001b[0m";
		}


		displayMenuGuide(true);

		option = _getch();
		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = 4;
				}
				break;
			case 80://down arrow    
				if (intOpt < 4) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				switch (intOpt)
				{
				case 0:
					displayInsertGuide(50, 1);
					MoveCursor(16, 2);
					set_cursor(true);
					nShop.ShopName = getchLine(50, 1, &nShop.ShopName);
					if (nShop.ShopName.empty() || nShop.ShopName.at(0) == '\0') {
						Error[0][0] = true;
					}
					else {
						Error[0][0] = false;
					}
					Error[0][1] = true;
					break;
				case 1:
					displayInsertGuide(11, 3);
					MoveCursor(16, 3);
					set_cursor(true);
					nShop.ContactNo = getchLine(11, 3, &nShop.ContactNo);
					if (nShop.ContactNo.size() < 10) {
						Error[1][0] = true;
					}
					else {
						Error[1][0] = false;
					}
					Error[1][1] = true;
					break;
				case 2:
					insertAddress(&nShop.address);
					if (!nShop.address.isValid()) {
						Error[2][0] = true;
					}
					else {
						Error[2][0] = false;
					}
					Error[2][1] = true;
					break;
				case 3:
					displayInsertGuide(255, 1);
					MoveCursor(16, 7);
					set_cursor(true);
					nShop.Description = getchLine(255, 1, &nShop.Description);
					if (nShop.Description.empty() || nShop.Description.at(0) == '\0') {
						Error[3][0] = true;
					}
					else {
						Error[3][0] = false;
					}
					Error[3][1] = true;
					break;
				case 4:
					if ((!Error[0][0] && Error[0][1]) && (!Error[1][0] && Error[1][1]) && (!Error[2][0] && Error[2][1]) && (!Error[3][0] && Error[3][1])) {
						if (shop != NULL) {
							shop->ShopName = nShop.ShopName;
							shop->Description = nShop.Description;
							shop->ContactNo = nShop.ContactNo;
							shop->address = nShop.address;
							shop->Update();
						}
						else {
							nShop.OwnerIC = user->IC;
							nShop.RegisterDate = strDateTimeNow();
							nShop.Register();
							if (!user->IsVendor) {
								user->UpdateVendorStatus(1);
							}
						}
						MainMenu(user);
					}
					else {

						if (nShop.ShopName.empty() || nShop.ShopName.at(0) == '\0') {
							Error[0][0] = true;
						}
						else {
							Error[0][0] = false;
						}
						Error[0][1] = true;
						if (nShop.ContactNo.size() < 10) {
							Error[1][0] = true;
						}
						else {
							Error[1][0] = false;
						}
						Error[1][1] = true;
						if (!nShop.address.isValid()) {
							Error[2][0] = true;
						}
						else {
							Error[2][0] = false;
						}
						Error[2][1] = true;
						if (nShop.Description.empty() || nShop.Description.at(0) == '\0') {
							Error[3][0] = true;
						}
						else {
							Error[3][0] = false;
						}
						Error[3][1] = true;
						break;
					}
					break;
				}
			}
			else if (option == 27) {
				return;
			}
		}

	} while (option != '\0');
}

void insertAddress(Address* address) {
	char option = ' ';
	int intOpt = 0;
	do {
		set_cursor(false);
		system("cls");
		cout << "Insert your address informations as follows\n" << endl;
		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">Street : " << address->Street << "\u001b[0m" << endl;
		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Region : " << address->Region << "\u001b[0m" << endl;
		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Postcode : " << address->PostCode << "\u001b[0m" << endl;
		if (intOpt == 3)
			cout << "\u001b[33m>";
		cout << ">Town/City : " << address->City << "\u001b[0m" << endl;
		if (intOpt == 4)
			cout << "\u001b[33m>";
		cout << ">State : " << address->State << "\u001b[0m" << endl;

		displayMenuGuide(true);

		option = _getch();
		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = 4;
				}
				break;
			case 80://down arrow    
				if (intOpt < 4) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				switch (intOpt) {
				case 0:
					displayInsertGuide(50, 1);
					MoveCursor(11, 2);
					set_cursor(true);
					address->Street = getchLine(50, 1, &address->Street);
					break;
				case 1:
					displayInsertGuide(25, 1);
					MoveCursor(11, 3);
					set_cursor(true);
					address->Region = getchLine(25, 1, &address->Region);
					break;
				case 2:
					displayInsertGuide(5, 3);
					MoveCursor(13, 4);
					set_cursor(true);
					address->PostCode = getchLine(5, 3, &address->PostCode);
					break;
				case 3:
					displayInsertGuide(20, 1);
					MoveCursor(14, 5);
					set_cursor(true);
					address->City = getchLine(20, 1, &address->City);
					break;
				case 4:
					displayInsertGuide(20, 1);
					MoveCursor(10, 6);
					set_cursor(true);
					address->State = getchLine(20, 1, &address->State);
					break;
				}
			}
			else if (option == 27) {
				return;
			}
		}
	} while (option != '\0');
}

void VendorMenu(Account* user) {
	ChangeScreen("Vendor Menu");
	char option = ' ';
	int intOpt = 0;
	do {
		system("cls");

		cout << "\033[4m_____Vendor Menu_____\033[0m" << endl;
		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">Manage Shop\u001b[0m" << endl;
		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Manage Orders\u001b[0m" << endl;
		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Analysis tool\u001b[0m" << endl;

		displayMenuGuide();
		option = _getch();
		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = 2;
				}
				break;
			case 80://down arrow    
				if (intOpt < 2) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				switch (intOpt) {
				case 0:
					FindShop(user, true);
					break;
				case 1:
					FindShop(user, true, 1);
					break;
				case 2://soon
					FindShop(user, true, 2);
					break;
				}
			}
			else if (option == 27) {
				MainMenu(user);
			}
		}
	} while (option != '\0');
}

bool ShopPage(Account* user, Shop* shop, Product* carryOverProduct) { //if any change = true
	ChangeScreen(shop->ShopName);
	bool isOwner = false;
	bool changes = false;
	bool productChanges = true;
	bool isSearching = false;
	bool forwardOption = false;
	bool isSelectingProduct = false;
	vector<Product>::iterator I;
	char c = ' ';
	int intOpt = 0;
	int selected = 0;
	int sortBy = 0;
	string strSortCat[] = { "Unsort","Price",  "Rating" };
	bool ASC = true;
	string keyName = "";
	vector<Product> productList;
	Product findProduct;
	findProduct.ShopId = shop->ShopId;
	Account owner;
	Orders newOrder(user, shop);
	bool skip = false;
	bool selectedFromFind = false;
	if (carryOverProduct != NULL)
		selectedFromFind = true;
	if (user->IC != shop->OwnerIC) {//declare outside so no need to query every loop
		owner = Account(shop->OwnerIC);
	}
	else {
		isOwner = true;
	}
	do {
		skip = false;
		system("cls");
		cout << "\033[4m" << shop->ShopName << "\033[0m" << endl;
		cout << "Contact Number  : " + shop->ContactNo << endl;
		cout << "Date Registered : " + shop->RegisterDate << endl;
		cout << "Address\t : " + shop->address.getPart1() << endl;
		cout << "\t  " + shop->address.getPart2();
		printLine('-');
		cout << endl;
		if (isOwner) { //user is the owner
			if (intOpt == 0)
				cout << "\u001b[33m>";
			cout << ">Add Product" << "\u001b[0m" << endl;
			if (intOpt == 1)
				cout << "\u001b[33m>";
			cout << ">Edit Shop" << "\u001b[0m" << endl;
			if (intOpt == 2)
				cout << "\u001b[33m>";
			cout << ">Delete Shop" << "\u001b[0m" << endl;
			if (intOpt == 3)
				cout << "\u001b[33m>";
			cout << ">Manage Product" << "\u001b[0m" << endl;
		}
		else {
			cout << "\033[4m" << "Owner Information :" << "\033[0m" << endl
				<< "Name      : " + owner.Name << endl
				<< "Phone No. : " + owner.PhoneNo;
		} 

		if (!isOwner || (isOwner && isSelectingProduct)) {
			printLine('-');
			if (productChanges) {
				//db op

				findProduct.Name = keyName;
				if (sortBy == 0) {
					productList = findProduct.Find();
				}
				else if (sortBy == 1) {
					productList = findProduct.SortByPrice(ASC);
				}
				else if (sortBy == 2) {
					productList = findProduct.SortByRating(ASC);
				}
				productChanges = false;
				selected = 0;
			}
			//carry over
			if (selectedFromFind) {
				int cI = LinerSearchProduct(&productList, carryOverProduct);
				if (cI != -1)
					selected = cI;
				selectedFromFind = false;
			}
			//===========Product Part======================
			cout << "\033[4m\nProducts List\033[0m" << endl;
			if (isSearching) { cout << "\u001b[33m>"; }
			cout << "Search Name: " << keyName << "\u001b[0m" << endl << "[ Found : " << productList.size() << " ]" << endl << "[Sort By : " << strSortCat[sortBy];
			if (sortBy > 0) {
				cout << "| Order : ";
				if (ASC)
					cout << "Lowest ~ Highest";
				else
					cout << "Higherst ~ Lowest";
			}
			cout << "]" << endl;


			if (!productList.empty()) {
				for (int i = 0; i < productList.size(); i++) {
					if ((selected - i <= 2 && selected - i >= -2)
						|| (selected == 1 && (selected - i <= 1 && selected - i >= -3)) // 2nd element
						|| (selected == 0 && (i - selected < 5 && i - selected >= 0))  // 1st element
						|| (selected == productList.size() - 2 && (selected - i <= 3 && selected - i >= -1)) //2nd last
						|| (selected == productList.size() - 1 && (selected - i >= 0 && selected - i < 5))) { // last element, all to ensure 5 display only

						if (i == selected) {
							cout << "\u001b[36m";
						}
						printLine('_');
						cout << endl << productList[i].Name << endl;
						cout << "Rating:[ "; 
						if (productList[i].Rating == 0) {
							cout << "NOT RATED YET";
						}
						else {
							if (productList[i].Rating >= 4.75)
								cout << "\u001b[33m";
							printf("%.2f", productList[i].Rating);
							if (productList[i].Rating >= 4.75)
								cout << "\u001b[0m";
							if (productList[i].Rating >= 4.75 && i == selected)
								cout << "\u001b[36m";
							cout << " / 5.00";
						}
						cout << " ]" << endl;
						if (productList[i].Description.size() > getConsoleSize().X) {
							cout << "-" << productList[i].Description.substr(0, getConsoleSize().X - 5) << "..." << endl;
						}
						else {
							cout << "-" << productList[i].Description << endl;
						}

						cout << "[Price : RM "; printf("%.2f", productList[i].Price); cout << " ] ";
						I = find(newOrder.Orderitems.begin(), newOrder.Orderitems.end(), productList[i]);
						if (I != newOrder.Orderitems.end()) {
							productList[i].Quantity = newOrder.Orderitems[distance(newOrder.Orderitems.begin(), I)].Quantity;
							cout << "[In Cart : " << productList[i].Quantity << " ]";
						}

						//product data
						printLine('_');
						if (i == selected) {
							cout << "\u001b[0m";
						}
					}
				}
			}
			cout << endl << endl << "\u001b[33m====GUIDE====\u001b[0m" << endl;

			if (isOwner) {
				cout << "--Press \u001b[33mRight Arrow/Enter\u001b[0m to Edit selected product" << endl;
			}
			else {
				cout << "--Press \u001b[33mRight/Left Arrow\u001b[0m key to add/remove 1 unit of selected product in cart" << endl
					<< "--Press \u001b[33mTab\u001b[0m to view product details" << endl << "--Press \u001b[33mEnter\u001b[0m to view your order" << endl;
			}
			if (isSearching) {
				cout << "--Press \u001b[33mBackspace\u001b[0m to delete a character in the filter" << endl;
				cout << "--Press \u001b[33mEscape(ESC)\u001b[0m to stop searching/editing name filter" << endl;
			}
			else {
				cout << "--Press \u001b[33m1\u001b[0m to insert/edit Product name filter" << endl;
				cout << "--Press \u001b[33m2\u001b[0m to change sort category" << endl;
				cout << "--Press \u001b[33m3\u001b[0m to set display order" << endl;
				if (isOwner)
					cout << "--Press \u001b[33mEscape(ESC)\u001b[0m to exit viewing product" << endl;
				else
					cout << "--Press \u001b[33mEscape(ESC)\u001b[0m to go back" << endl;
			}
			//============================================================
			if (!forwardOption) {
				c = _getch();
			}
			else {
				forwardOption = false;
			}

			if (int(c) == -32) { //special char eg:arrow
				productChanges = false;
				c = _getch();
				switch (c) {
				case 72: //Up Arrow   
					if (selected > 0) {
						selected--;
					}
					break;
				case 80://down arrow    
					if (selected < productList.size() - 1) {
						selected++;
					}
					break;
				case 77: //right arrow
					if (isOwner && !productList.empty()) {
						productChanges = ProductDetails(user, shop, &productList[selected]);
					}
					if (!isOwner && !productList.empty()) {
						newOrder.addProduct(productList[selected]);
					}
					break;
				case 75://left arrow
					if (!isOwner && !newOrder.Orderitems.empty()) {
						newOrder.removeProduct(productList[selected]);
					}
					break;
				default:
					break;
				}
			}//==
			else {
				switch (c) {
				case 72: //Up Arrow   
					if (selected > 0) {
						selected--;
					}
					break;
				case 80://down arrow    
					if (selected < productList.size() - 1) {
						selected++;
					}
					break;
				case 77: //right arrow
					if (isOwner && !productList.empty()) {
						productChanges = ProductDetails(user, shop, &productList[selected]);
					}
					if (!isOwner && !productList.empty()) {
						if (newOrder.Orderitems.empty()) {
							newOrder.OrderDate = strDateTimeNow();
						}
						newOrder.addProduct(productList[selected]);
					}
					break;
				case 75://left arrow
					if (!isOwner && !newOrder.Orderitems.empty()) {
						newOrder.removeProduct(productList[selected]);
					}
					break;
				case '\t':
					if (!isOwner && !productList.empty()) {
						productChanges = ProductDetails(user, shop, &productList[selected]);
					}
					break;
				case '\r':
					if (isSearching) {
						isSearching = false;
					}
					else {
						if (isOwner && isSelectingProduct && !productList.empty()) {
							productChanges = ProductDetails(user, shop, &productList[selected]);
						}
						if (!isOwner && !newOrder.Orderitems.empty()) {
							newOrder.CustContactNo = user->PhoneNo;
							newOrder.ShopContactNo = shop->ContactNo;
							newOrder.CustomerIC = user->IC;
							newOrder.OrderDate = strDateTimeNow();
							OrderDetails(user, &newOrder);
						}
					}
					break;
				case 27:
					if (isSearching) {
						isSearching = false;
						break;
					}
					if (isOwner && isSelectingProduct) {
						isSelectingProduct = false;
						skip = true;
						break;
					}
					return changes;
					break;
				case '\b':
					if (isSearching) {
						if (keyName.size() > 0) {
							keyName.resize(keyName.size() - 1);
							productChanges = true;
						}
					}
					break;
				case '1':
					if (!isSearching) {
						isSearching = true;
						break;
					}
				case '2':
					if (!isSearching) {
						productChanges = true;
						if (sortBy == 2)
							sortBy = 0;
						else
							sortBy++;
						break;
					}
				case '3':
					if (!isSearching && sortBy != 0) {
						productChanges = true;
						if (ASC)
							ASC = false;
						else
							ASC = true;
						break;
					}
				default:
					if (isSearching) {
						//varchars char + num + sum special char                                  space          comma            dot               numbers
						if ((int(c) >= 65 && int(c) <= 90) || (int(c) >= 97 && int(c) <= 122) || int(c) == 32 || int(c) == 44 || int(c) == 46 || (int(c) >= 48 && int(c) <= 57)) {
							if (keyName.size() < 50) {
								keyName += c;
								productChanges = true;
							}
						}

					}
					else {
						productChanges = false; //if not inserting aka still in find shop main window but user insert unnecessary char, no change so no ned to requery at top
						do {
							c = _getch();
							if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '\r' || c == 27) {
								break;
							}
							if (int(c) == -32) {
								c = _getch();
								if (c == 72 || c == 80) {
									break;
								}
							}
						} while (c != '\0');
						forwardOption = true;
					}
					break;
				}
			}


		}


		if (isOwner && !isSelectingProduct && !skip) {
			displayMenuGuide();
			c = _getch();
			if (int(c) == -32) { //special char eg:arrow 
				c = _getch();
				switch (c) {
				case 72: //Up Arrow   
					if (intOpt > 0) {
						intOpt--;
					}
					else {
						intOpt = 3;
					}
					break;
				case 80://down arrow    
					if (intOpt < 3) {
						intOpt++;
					}
					else {
						intOpt = 0;
					}
					break;
				}
			}
			else {
				if (c == '\r') {
					switch (intOpt) {
					case 0:
						ProductDetails(user, shop);
						changes = true;
						break;
					case 1:
						RegisterShop(user, shop);
						changes = true;
						break;
					case 2:
						cout << endl << "\u001b[41m\u001b[37m*** Warning : Deleting " << shop->ShopName << " will also delete all product registered in it ***\u001b[0m";
						cout << endl << "Press \u001b[33mEnter\u001b[0m to proceed or \u001b[33mEscape(ESC)\u001b[0m to cancel";
						while (c != '\0') {
							c = _getch();
							switch (c) {
							case  '\r':
								system("cls");
								shop->Delete();
								user->ReEvaluateVendorStatus();
								changes = true;
								cout << shop->ShopName << " has been deleted" << endl << "Press any key to return";
								_getch();
								if (user->IsVendor)
									return changes;
								else
									MainMenu(user);
								break;
							case 27:
								c = '\0';
								break;
							}
						}
						c = ' ';
						break;
					case 3:
						isSelectingProduct = true;
						productChanges = true; //trigger reload from db
						break;
					}
				}
				else if (c == 27) {
					return changes;
				}
			}
		}

	} while (c != '\0');
}


void FindAllProduct(Account* user) {
	ChangeScreen("Find Product");
	Product searchController;
	vector<Product> searchRes;
	Shop selectedShop;
	string keyName;
	string tmpKeyName;
	char c = ' ';
	int selected = 0;
	int sortBy = 0;
	string strSortCat[] = { "Unsort","Price",  "Rating" };
	bool isSearching = false;
	bool ASC = true;
	bool forwardOption = false;
	bool refreshSearch = true;;
	while (1) {
		//db op here
		if (refreshSearch) {
			//db op

			searchController.Name = keyName;
			if (sortBy == 0) {
				searchRes = searchController.getAllProduct();
			}
			else if (sortBy == 1) {
				searchRes = searchController.getAllProductSortByPrice(ASC);
			}
			else if (sortBy == 2) {
				searchRes = searchController.getAllProcuctSortByRating(ASC);
			}
			refreshSearch = false;
			selected = 0;
		}
		system("cls");
		cout << "\033[4m\nProducts List\033[0m" << endl;
		if (isSearching)  
			cout << "\u001b[33m>"; 
		cout << "Search Name: " << keyName << "\u001b[0m" << endl << "[ Found : " << searchRes.size() << " ]" << endl << "[Sort By : " << strSortCat[sortBy];
		if (sortBy > 0) {
			cout << "| Order : ";
			if (ASC)
				cout << "Lowest ~ Highest";
			else
				cout << "Higherst ~ Lowest";
		}
		cout << "]" << endl;
		 
		printLine('=');
		cout << endl;

		//display here
		if (!searchRes.empty()) {
			for (int i = 0; i < searchRes.size(); i++) {
				if ((selected - i <= 2 && selected - i >= -2)
					|| (selected == 1 && (selected - i <= 1 && selected - i >= -3)) // 2nd element
					|| (selected == 0 && (i - selected < 5 && i - selected >= 0))  // 1st element
					|| (selected == searchRes.size() - 2 && (selected - i <= 3 && selected - i >= -1)) //2nd last
					|| (selected == searchRes.size() - 1 && (selected - i >= 0 && selected - i < 5))) { // last element, all to ensure 5 display only

					if (i == selected) {
						cout << "\u001b[36m";
					}
					printLine('_');
					cout << endl << searchRes[i].Name << endl;
					cout << "Sold by : " << searchRes[i].ShopName << endl;
					cout << "Rating:[ ";
					if (searchRes[i].Rating == 0) {
						cout << "NOT RATED YET";
					}
					else {
						if (searchRes[i].Rating >= 4.75)
							cout << "\u001b[33m";
						printf("%.2f", searchRes[i].Rating);
						if (searchRes[i].Rating >= 4.75)
							cout << "\u001b[0m";
						if (searchRes[i].Rating >= 4.75 && i == selected)
							cout << "\u001b[36m";
						cout << " / 5.00";
					}
					cout << " ]" << endl;
					if (searchRes[i].Description.size() > getConsoleSize().X) {
						cout << "-" << searchRes[i].Description.substr(0, getConsoleSize().X - 5) << "..." << endl;
					}
					else {
						cout << "-" << searchRes[i].Description << endl;
					}

					cout << "[Price : RM "; printf("%.2f", searchRes[i].Price); cout << " ] "; 
					//product data
					printLine('_');
					if (i == selected) {
						cout << "\u001b[0m";
					}
				}
			}
		}
		printLine('=');
		cout << endl;

		//guide 
 
		if (isSearching) {
			cout << endl << endl << "\u001b[33m====SEARCHING====\u001b[0m" << endl;
			cout << "--Press \u001b[33mEnter\u001b[0m to finish" << endl;
			cout << "--Press \u001b[33mBackspace\u001b[0m to delete a character in the filter" << endl;
			cout << "--Press \u001b[33mEscape(ESC)\u001b[0m to stop searching/editing name filter" << endl;
		}
		else {
			cout << endl << endl << "\u001b[33m====GUIDE====\u001b[0m" << endl;
			cout << "--Press \u001b[33mUp/Down\u001b[0m arrow to move selection and scroll the view" << endl;
			cout << "--Press \u001b[33m1\u001b[0m to insert/edit Product name filter" << endl;
			cout << "--Press \u001b[33m2\u001b[0m to change sort category" << endl;
			cout << "--Press \u001b[33m3\u001b[0m to set display order" << endl;
			cout << "--Press \u001b[33mEnter\u001b[0m to go to the selected product shop" << endl;
			cout << "--Press \u001b[33mTab\u001b[0m to view product details" << endl;
			cout << "--Press \u001b[33mEscape(ESC)\u001b[0m to go back to main menu" << endl;
		}

		if (!forwardOption) {
			c = _getch();
		}
		else {
			forwardOption = false;
		}

		if (int(c) == -32) { //special char eg:arrow 
			c = _getch();
			switch (c) {
			case 72: //Up Arrow   
				if (selected > 0) {
					selected--;
				}
				break;
			case 80://down arrow    
				if (selected < searchRes.size() - 1) {
					selected++;
				}
				break;  
			}
		}
		else {
			switch (c) {
			case 72: //Up Arrow   
				if (selected > 0) {
					selected--;
				}
				break;
			case 80://down arrow    
				if (selected < searchRes.size() - 1) {
					selected++;
				}
				break; 
			case '\t'://go to detail
				if (!isSearching && searchRes.size() > 0) {
					selectedShop = Shop(searchRes[selected].ShopId);
					refreshSearch = ProductDetails(user, &selectedShop, &searchRes[selected]);
				}
				break;
			case '\r': 
				if (isSearching) {
					isSearching = false;
					refreshSearch = true;
					break;
				}
				else if (searchRes.size() > 0) {
					selectedShop = Shop(searchRes[selected].ShopId);
					ShopPage(user, &selectedShop, &searchRes[selected]);
					refreshSearch = true;
				} 
				break;
			case 27:
				if (isSearching) {
					isSearching = false;
					keyName = tmpKeyName; 
					break;
				}
				else {
					MainMenu(user);
				}
				break;
			case '\b':
				if (isSearching) {
					if (keyName.size() > 0) {
						keyName.resize(keyName.size() - 1); 
					}
				}
				break;
			case '1':
				if (!isSearching) {
					isSearching = true;
					tmpKeyName = keyName;
					break;
				}
			case '2':
				if (!isSearching) {
					refreshSearch = true;
					if (sortBy == 2)
						sortBy = 0;
					else
						sortBy++;
					break;
				}
			case '3':
				if (!isSearching && sortBy != 0) {
					refreshSearch = true;
					if (ASC)
						ASC = false;
					else
						ASC = true;
					break;
				}
			default:
				if (isSearching) {
					//varchars char + num + sum special char                                  space          comma            dot               numbers
					if ((int(c) >= 65 && int(c) <= 90) || (int(c) >= 97 && int(c) <= 122) || int(c) == 32 || int(c) == 44 || int(c) == 46 || (int(c) >= 48 && int(c) <= 57)) {
						if (keyName.size() < 50) {
							keyName += c; 
						}
					}

				}
				else {
					refreshSearch = false; //if not inserting aka still in find shop main window but user insert unnecessary char, no change so no ned to requery at top
					do {
						c = _getch();
						if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '\r' || c == 27) {
							break;
						}
						if (int(c) == -32) {
							c = _getch();
							if (c == 72 || c == 80) {
								break;
							}
						}
					} while (c != '\0');
					forwardOption = true;
				}
				break;
			}
		}



	}


}

bool ProductDetails(Account* user, Shop* shop, Product* product, bool viewOnly, bool fromOrder) { //return true if theres changes to product in param
	Product nProduct;
	if(shop != NULL)
		nProduct.ShopId = shop->ShopId;
	bool userInput[3] = { false };
	bool error = false;
	bool isOwner = true;
	int maxOpt = 3;
	bool isEdit = false;
	bool Rate = false;
	if (product != NULL) {
		maxOpt = 5;
		nProduct = *product;
		userInput[0] = true;
		userInput[1] = true;
		userInput[2] = true;
		if (user->IC != shop->OwnerIC) {
			isOwner = false;
		}
		isEdit = true;
	}
	ChangeScreen("Product Details");
	char option = ' ';
	int intOpt = 0;
	if (!isOwner) {
		intOpt = -1;
	}
	string strPrice;
	do {  
		system("cls");
		set_cursor(false);
		cout << "Shop \t: " << shop->ShopName << endl;
		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">Product Name : " << nProduct.Name << "\u001b[0m" << endl;
		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Price(RM) : "; printf("%.2f", nProduct.Price);  cout << "\u001b[0m" << endl;
		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Description : " << endl << nProduct.Description << "\u001b[0m";
		generateVerticalSpace(255);
		cout << "Rating\t: ";
		if (nProduct.Rating == 0) {
			cout << "NOT RATED YET" << endl;
		}
		else {
			printf("%.2f", nProduct.Rating);
			cout << "/5.00" << endl;
		}
		
		if (isOwner) {
			if (intOpt == 3)
				cout << "\u001b[33m>";
			cout << ">Save changes" << "\u001b[0m" << endl;
			if (product != NULL) {
				if (intOpt == 4)
					cout << "\u001b[33m>";
				cout << ">Remove product" << "\u001b[0m" << endl;
				if (intOpt == 5)
					cout << "\u001b[33m>";
				cout << ">View product rating & comments" << "\u001b[0m" << endl;
			}
		}


		//============vendor access only
		if (isOwner && !viewOnly) {
			//Validations
			displayMenuGuide(true);
			error = false;
			if (userInput[0] && (nProduct.Name.empty() || nProduct.Name.at(0) == ' ')) {
				cout << "\u001b[41;1m\u001b[37m\n- Error : Product Name must not be empty\u001b[0m";
				error = true;
			}
			if (userInput[1] && nProduct.Price <= 0) {
				cout << "\u001b[41;1m\u001b[37m\n- Error : Product Price must be greater than 0\u001b[0m";
				error = true;
			}
			if (userInput[2] && (nProduct.Description.empty() || nProduct.Description.at(0) == ' ')) { //desc
				cout << "\u001b[41;1m\u001b[37m\n- Error : Product Description must not be empty\u001b[0m";
				error = true;
			}
			if (userInput[0] && userInput[1] && userInput[2] && !error) {
				cout << "\u001b[42;1m\u001b[37m\n  Valid informations  \u001b[0m";
			}

			option = _getch();
			if (int(option) == -32) { //special char eg:arrow 
				option = _getch();
				switch (option) {
				case 72: //Up Arrow   
					if (intOpt > 0) {
						intOpt--;
					}
					else {
						intOpt = maxOpt;
					}
					break;
				case 80://down arrow    
					if (intOpt < maxOpt) {
						intOpt++;
					}
					else {
						intOpt = 0;
					}
					break;
				}
			}
			else {
				if (option == '\r') {
					switch (intOpt) {
					case 0:
						displayInsertGuide(50, 1);
						MoveCursor(17, 1);
						set_cursor(true);
						nProduct.Name = getchLine(50, 1, &nProduct.Name);

						userInput[0] = true;
						break;
					case 1:
						MoveCursor(14, 2);
						set_cursor(true);
						strPrice = "";
						cout << "         \b\b\b\b\b\b\b\b\b";
						strPrice = getchLine(6, 4, &strPrice, 2); //in db 8,2 ; 6 for digit, function will detect . and add max when necessary   
						if (!strPrice.empty())	nProduct.Price = myStrToFloat(&strPrice);
						userInput[1] = true;
						break;
					case 2:
						displayInsertGuide(255, 1);
						MoveCursor(0, 4);
						set_cursor(true);
						nProduct.Description = getchLine(255, 1, &nProduct.Description);
						userInput[2] = true;
						break;
					case 3:
						if (userInput[0] && userInput[1] && userInput[2] && !error) {
							if (product == NULL) {
								nProduct.Register();
							}
							else {
								if (product->Name == nProduct.Name && product->Description == nProduct.Description && product->Price == nProduct.Price) {
									cout << endl << "No change detected" << endl;
									_getch();
									break;
								}

								nProduct.Update();
							}
							system("cls");
							if (!isEdit)
								cout << nProduct.Name << " has been added into " << shop->ShopName << " shop";
							else
								cout << "Product detail has benn updated";
							cout << endl << "Press any key to go back to product list";
							_getch();
							return true;
						}
						break;
					case 4:
						if (product != NULL) {
							cout << endl << endl<< "\u001b[41m\u001b[37m*** Warning : Deleting Product: " << nProduct.Name << " ***\u001b[0m";
							cout << endl << "Press Enter to proceed or escape to cancel";
							option = _getch();
							switch (option) {
							case  '\r':
								system("cls");
								nProduct.Delete();
								cout << nProduct.Name << " has been deleted" << endl << "Press any key to return";
								_getch();
								return true;
								break;
							case 27:
								option = ' ';
								break;
							}
						}
						break;
					case 5:
						if (product != NULL) { 
							ViewAllRating(user, product);
						}
						break;
					}

				}
				else if (option == 27) {
					return false;
				}
			}


		}
		else {
			bool viewAllrateOnly = false;
			bool eligilble = product->isEligibleRate(user->IC);
			if (fromOrder) {
				cout << endl << "NOTE : Product detail may have changed by the vendor if it is different from your oder details" << endl;
			}
			if (!isOwner && eligilble) {
				cout << endl << "Press \u001b[33mEnter\u001b[0m to go to rating & comments" << endl;
				Rate = true;
			}
			else if (!isOwner && !eligilble) {
				cout << endl << "Press \u001b[33mEnter\u001b[0m to view product rating & comments" << endl;
			}
			cout << endl << "--Press \u001b[33many key\u001b[0m to go back--" << endl;
			option = _getch();
			if (option == '\r') {
				if (Rate) {
					RateProduct(user, product);
					nProduct.RefreshRating();
				}
				else if (!isOwner && !eligilble) {
					ViewAllRating(user, product);
				}
			}
			else {
				if (Rate)
					return true;
				else
					return false;
			}

			//customer option later add rating by customer here
		}
	} while (option != '\0');
}

void RateProduct(Account* user, Product* productToRate) {
	ChangeScreen("Product Rating & Comments");
	char option = ' ';
	int intOpt = 0;
	int myRate = 1;
	string myComment = "";
	bool changes = true;
	ProductRating myRating; 
	while (1) {
		set_cursor(false);
		if (changes) {
			myRating = ProductRating(productToRate->ProductId, user->IC);
			if (myRating.ProductId != -1) {
				myRate = myRating.Rating;
				myComment = myRating.Comment;
			}
			changes = false;
		}
		system("cls");
		cout << endl << "Product: " << productToRate->Name << " Ratings" << endl;
		printLine('-');
		cout << endl<< "Your Rating" << endl;

		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">Rating(1~5) : " << myRate << "\u001b[0m" << endl;

		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Comment: " << myComment << "\u001b[0m" << endl;

		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Save Rating" << "\u001b[0m" << endl;
		if (intOpt == 3)
			cout << "\u001b[33m>";
		cout << ">See Others Ratings & Comments" << "\u001b[0m" << endl;
		displayMenuGuide(true);
		option = _getch();
		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = 3;
				}
				break;
			case 80://down arrow    
				if (intOpt < 3) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				if (intOpt == 0) {
					cout << endl << "--Press any number from 1 to 5 to rate" << endl;
					MoveCursor(16, 5);
					set_cursor(true);
					char temp = _getch();
					if ((int)temp >= 49 && (int)temp <= 53) {
						myRate = temp - '0';
					}
				}
				else if (intOpt == 1) {
					displayInsertGuide(100, 1);
					MoveCursor(11, 6);
					set_cursor(true);
					myComment = getchLine(100, 1, &myComment);
				}
				else if (intOpt == 2) {
					if (myComment != myRating.Comment || myRate != myRating.Rating) {
						changes = true;
						if (myRating.ProductId == -1) {//new insert
							myRating.CustomerIC = user->IC;
							myRating.ProductId = productToRate->ProductId;
							myRating.Comment = myComment;
							myRating.Rating = myRate;
							myRating.Insert(); 
							system("cls");
							cout << endl << endl << "\u001b[33mRating Saved\u001b[0m -- Press ny key to continue" << endl;
							_getch();
						}
						else {
							myRating.Comment = myComment;
							myRating.Rating = myRate;
							myRating.Update();
							system("cls");
							cout << endl << endl << "\u001b[33mRating Updated\u001b[0m -- Press ny key to continue" << endl;
							_getch();
						}
						
					}
				}
				else if (intOpt == 3) {//go to all rating
					ViewAllRating(user, productToRate);
				}
			}
			else if (option == 27) {
				return;
			}
		}

	}
}

void ViewAllRating(Account* user, Product* product) {
	ChangeScreen("All rating & comment for " + product->Name);
	product->RefreshRating();
	vector<ProductRating> allRating = product->getAllRating();
	char option = ' ';
	int intOpt = 0;
	if (allRating.size() == 0) {
		cout << "No rating available yet for this product\nPress any key to go back";
		_getch();
		return;
	}
	else {
		while (1) {
			system("cls");
			cout << "=== All Rating & Comment for " << product->Name << " ===" << endl;
			cout << "Found " << allRating.size() << " comment & ratings by customers" << endl;
			cout << "Total average rating : ";
			printf("%.2f", product->Rating);
			cout << endl;
			for (int i = 0; i < allRating.size(); i++) {
				if ((intOpt - i <= 2 && intOpt - i >= -2)
					|| (intOpt == 1 && (intOpt - i <= 1 && intOpt - i >= -3)) // 2nd element
					|| (intOpt == 0 && (i - intOpt < 5 && i - intOpt >= 0))  // 1st element
					|| (intOpt == allRating.size() - 2 && (intOpt - i <= 3 && intOpt - i >= -1)) //2nd last
					|| (intOpt == allRating.size() - 1 && (intOpt - i >= 0 && intOpt - i < 5))) { // last element, all to ensure 5 display only


					if (intOpt == i)
						cout << "\u001b[36m";
					printLine('_');
					if (allRating[i].CustomerIC == user->IC) {
						cout << endl << "Your Rating "  << endl;
					}
					else {
						cout << endl << "Customer Name : " << allRating[i].CustomerName << endl;
					}
					
					cout << "Rating : " << allRating[i].Rating << " / 5" << endl
						<< "Comment : " << allRating[i].Comment << endl;
					printLine('_');
					if (intOpt == i)
						cout << "\u001b[0m";

				}
			}


			cout << endl << endl << "Press \u001b[33mUp/Down\u001b[0m Arrow key to scroll the view" << endl;
			cout << "Press \u001b[33mEscape(ESC)\u001b[0m to go back" << endl;

			option = _getch();

			if (int(option) == -32) { //special char eg:arrow 
				option = _getch();
				switch (option) {
				case 72: //Up Arrow   
					if (intOpt > 0) {
						intOpt--;
					} 
					break;
				case 80://down arrow    
					if (intOpt < allRating.size() - 1) {
						intOpt++;
					} 
					break;
				}
			}
			else {
				if (option == 27) {
					return;
				}
			}
		}
	}
}

void Register() {
	ChangeScreen("Registration");
	string ic, name, phone, password = "\0";
	bool Error[4][2] = { false };//1st row for error 2nd row to make sure if user has try inputting 
	char option = ' ';
	int intOpt = 0;

	while (option != '\0') {
		set_cursor(false);
		system("cls");
		cout << "Please fill in the following information to register" << endl << "Select confirm and proceed after all valid information has been inserted" << endl;
		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">IC Number : " << ic << "\u001b[0m" << endl;
		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Name: " << name << "\u001b[0m" << endl;
		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Phone number : " << phone << "\u001b[0m" << endl;
		if (intOpt == 3)
			cout << "\u001b[33m>";
		cout << ">Password: " << password << "\u001b[0m" << endl;
		if (intOpt == 4)
			cout << "\u001b[33m>";
		cout << ">Confirm and Proceed\u001b[0m" << endl;
		cout << "Press Escape(ESC) to cancel and go back to previous menu)" << endl;

		if (Error[0][0] == true) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid Identification Number(IC) \u001b[0m";
		}
		if (Error[1][0] == true) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Please fill in your name \u001b[0m";
		}
		if (Error[2][0] == true) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid Phone Number \u001b[0m";
		}
		if (Error[3][0] == true) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid password, Must contain 6 to 12 character \u001b[0m";
		}
		if ((!Error[0][0] && Error[0][1]) && (!Error[1][0] && Error[1][1]) && (!Error[2][0] && Error[2][1]) && (!Error[3][0] && Error[3][1])) {
			cout << "\u001b[42;1m\u001b[37m\n  Please confirm your informations and press enter to proceed  \u001b[0m";
		}

		displayMenuGuide(true);
		option = _getch();

		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = 4;
				}
				break;
			case 80://down arrow    
				if (intOpt < 4) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				switch (intOpt) {
				case 0:
					displayInsertGuide(12, 3);
					MoveCursor(14, 2);
					set_cursor(true);
					ic = getchLine(12, 3, &ic);
					if (ic.size() != 12) {
						Error[0][0] = true;
					}
					else {
						Error[0][0] = false;
					}
					Error[0][1] = true;
					break;
				case 1:
					displayInsertGuide(100, 2);
					MoveCursor(8, 3);
					set_cursor(true);
					name = getchLine(100, 2, &name);
					if (name.size() == 0) {
						Error[1][0] = true;
					}
					else {
						Error[1][0] = false;
					}
					Error[1][1] = true;

					break;
				case 2:
					displayInsertGuide(11, 3);
					MoveCursor(17, 4);
					set_cursor(true);
					phone = getchLine(11, 3, &phone);
					if (phone.size() == 0 || phone.size() < 10) {
						Error[2][0] = true;
					}
					else {
						Error[2][0] = false;
					}
					Error[2][1] = true;
					break;
				case 3:
					displayInsertGuide(12, 1);
					MoveCursor(12, 5);
					set_cursor(true);
					password = getchLine(12, 1, &password);
					if (password.size() < 6) {
						Error[3][0] = true;
					}
					else {
						Error[3][0] = false;
					}
					Error[3][1] = true;

					break;
				case 4:
					if ((!Error[0][0] && Error[0][1]) && (!Error[1][0] && Error[1][1]) && (!Error[2][0] && Error[2][1]) && (!Error[3][0] && Error[3][1])) {

						Account newUser;
						newUser.IC = ic;
						newUser.Name = name;
						newUser.Password = password;
						newUser.PhoneNo = phone;
						newUser.RegisterDate = strDateTimeNow();

						int reg = newUser.Register();
						if (reg == 0) {
							ChangeScreen("Registration Success!");
							cout << "Successfully registered\nPress ENTER to proceed to login page";

						}
						else if (reg == 1) {
							ChangeScreen("Registration Failure!");
							cout << "Identification number(IC):" + ic + "is already registered in the system";
						}
						cout << "\nPress Escape(ESC) to exit to menu";
						while (option != '\0') {
							option = _getch();
							switch (option) {
							case 13://login
								if (reg == 0)Login();
								return;
								break;
							case 27: option = '\0';
								ChangeScreen(" ");
							default:
								break;
							}
						}
					}
					else {
						if (password.size() < 6) {
							Error[3][0] = true;
						}
						else {
							Error[3][0] = false;
						}
						Error[3][1] = true;
						if (phone.size() == 0 || phone.size() < 10) {
							Error[2][0] = true;
						}
						else {
							Error[2][0] = false;
						}
						Error[2][1] = true;
						if (name.size() == 0) {
							Error[1][0] = true;
						}
						else {
							Error[1][0] = false;
						}
						Error[1][1] = true;
						if (ic.size() != 12) {
							Error[0][0] = true;
						}
						else {
							Error[0][0] = false;
						}
						Error[0][1] = true;
					}
					break;
				}
			}
			else if (option == 27) {
				return;
			}
		}
	}
}

void Login() {
	ChangeScreen("Login");
	string ic, password = "\0";
	char option = ' ';
	int intOpt = 0;
	bool error[2] = { false };
	int Rowcount = -1;
	while (option != '\0') {
		system("cls");
		cout << "Please enter your credentials to log into the system" << endl << endl;
		if (intOpt == 0)
			cout << "\u001b[33m>";
		cout << ">IC number : " << ic << "\u001b[0m" << endl;
		if (intOpt == 1)
			cout << "\u001b[33m>";
		cout << ">Password  : " << password << "\u001b[0m" << endl;
		if (intOpt == 2)
			cout << "\u001b[33m>";
		cout << ">Login" << "\u001b[0m" << endl;
		if (error[0]) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid Identification Number(IC) \u001b[0m";
		}
		if (error[1]) {
			cout << "\u001b[41;1m\u001b[37m\n Error : Invalid password, Must contain 6 to 12 character \u001b[0m";
		}
		if (ic.size() == 12 && password.size() >= 6) {
			cout << "\u001b[42;1m\u001b[37m\n  Please confirm your informations and select Login to proceed  \u001b[0m";
		}

		displayMenuGuide(true);
		option = _getch();
		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				else {
					intOpt = 2;
				}
				break;
			case 80://down arrow    
				if (intOpt < 2) {
					intOpt++;
				}
				else {
					intOpt = 0;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				switch (intOpt) {
				case 0:
					displayInsertGuide(12, 3);
					MoveCursor(14, 2);
					set_cursor(true);
					ic = getchLine(12, 3, &ic);
					if (ic.size() != 12) {
						error[0] = true;
					}
					else {
						error[0] = false;
					}
					break;
				case 1:
					displayInsertGuide(12, 1);
					MoveCursor(14, 3);
					set_cursor(true);
					password = getchLine(12, 1, &password);
					if (password.size() < 6) {
						error[1] = true;
					}
					else {
						error[1] = false;
					}
					break;
				case 2:
					if (ic.size() == 12 && password.size() >= 6) {
						Account user;
						user.IC = ic;
						user.Password = password;
						if (user.Login()) {
							MainMenu(&user);
							return;
						}
						else {
							ChangeScreen("Login Failed");
							cout << "\u001b[41;1m\u001b[37m\nError : Invalid credentials, please try again.\u001b[0m";
							cout << "\nPress any key to go back to home page";
							_getch();
							return;
						}
					}
					else {
						if (ic.size() != 12)
							error[0] = true;
						if (password.size() < 6)
							error[1] = true;
					}
					break;
				}

			}
			else if (option == 27) {
				return;
			}

		}
		set_cursor(false);
	}

}

bool OrderDetails(Account* user, Orders* order) { //retrun true if theres change
	ChangeScreen("Order Details");
	char option = ' ';
	char confirm = '\0';
	int intOpt = 0;
	int maxOpt;
	bool isEditing = false;
	bool isVendor = false;
	bool changes = false;
	float payment;
	Orders thisOrder = *order;
	string status[] = { "New", "Accepted" , "Ready" ,"Canceled", "Refused" , "Failed" , "Completed","Pending Payment" };

	Product selectedOrderProduct;
	Shop selectedorderShop;
	string comment;
	do {
		if (!(thisOrder.CustomerIC == user->IC))
			isVendor = true;
		system("cls");
		if (!isVendor && thisOrder.Status > 0)
			isEditing = true;
		if (isVendor && thisOrder.Status >= 3 && thisOrder.Status <= 6) //view only status
			isEditing = true;
		if (!thisOrder.Orderitems.empty()) {
			cout << "\033[4mShop info\t\t\t\033[0m" << endl;
			cout << "Shop Name \t: " << thisOrder.ShopName << endl;
			cout << "Contact \t: " << thisOrder.ShopContactNo << endl;
			cout << "\033[4m\nCustomer info\t\t\t\033[0m" << endl;
			cout << "Customer IC\t: " << thisOrder.CustomerIC << endl;
			cout << "Customer Name\t: " << thisOrder.CustName << endl;
			cout << "Contact \t: " << thisOrder.CustContactNo << endl;
			cout << "\033[4m\nOrder Details\t\t\t\033[0m" << endl;

			if (thisOrder.Status >= 0) {
				cout << "Order Id \t: " << thisOrder.OrderId << endl;
			}
			cout << "Order Date \t: " << thisOrder.OrderDate << endl;
			if (thisOrder.Status != -1) {
				cout << "Status \t\t: " << status[thisOrder.Status] << endl;
			}
			if (thisOrder.Status == 6) {
				cout << "Complete Date \t: " << thisOrder.CompleteDate << endl;
			}
			if (thisOrder.Status == 7) {
				cout << "Payment Recieved : RM "; printf("%.2f", thisOrder.PaymentRecieved); cout << endl;
				cout << "Pending payment  : RM "; printf("%.2f", (thisOrder.TotalPrice - thisOrder.PaymentRecieved)); cout << endl;
			}
			cout << "\033[4m";
			printf("\n______________________________________________________________________"); //70 character long
			cout << "\u001b[46m\u001b[30m";
			printf("\n%-50s|%9s|%8s|", "Product Name", "Price(RM)", "Quantity");
			int i = 0;
			if (isEditing && !thisOrder.Orderitems.empty()) {
				maxOpt = thisOrder.Orderitems.size() - 1;
			}
			for (Product tmpProd : thisOrder.Orderitems) {
				cout << "\u001b[47m\u001b[30m\033[4m"; //cyan bg black text underline
				if (i == intOpt && isEditing) {
					cout << "\u001b[44m\u001b[37m"; //blue bg white text
				}

				printf("\n%-50s|%9.2f|%8d|", tmpProd.Name.c_str(), tmpProd.Price, tmpProd.Quantity);
				i++;
				if (i == intOpt && isEditing) {
					cout << "\u001b[0m";
				}
			}
			cout << "\u001b[46m\u001b[30m";
			printf("\n%-50s|%18.2f|", "Total Price(RM)", thisOrder.TotalPrice);
			cout << "\033[0m";

			if (thisOrder.Status == 5 || thisOrder.Status == 6) {
				cout << endl << "Comment : " << ParseComment(thisOrder.Comment);
			}

		}

		if (thisOrder.Status == -1) { //new and not yet inserted into DB
			if (thisOrder.Orderitems.empty()) {
				order->Orderitems.clear();
				return false;
			}
			if (!isEditing)
				maxOpt = 3;
			cout << endl;
			if (intOpt == 0 && !isEditing)
				cout << "\u001b[33m>";
			cout << ">Place Order" << "\u001b[0m" << endl;
			if (intOpt == 1 || isEditing)
				cout << "\u001b[33m>";
			cout << ">Edit order Items" << "\u001b[0m" << endl;
			if (intOpt == 2 && !isEditing)
				cout << "\u001b[33m>";
			cout << ">Add more Product/go back" << "\u001b[0m" << endl;
			if (intOpt == 3 && !isEditing)
				cout << "\u001b[33m>";
			cout << ">Cancel/Empty Order" << "\u001b[0m" << endl;

			cout << endl;
			if (!isEditing) {
				displayMenuGuide();
			}
			else {
				cout << endl << "--Edit Order Mode--" << endl << "-Press \u001b[33mUp/Down\u001b[0m Arrow key to select a product" << endl;
				cout << "-Press \u001b[33mLeft/Right\u001b[0m Arrow key to decrease/increase quantity of selected product" << endl;
				cout << "-Press \u001b[33mEnter\u001b[0m to save changes" << endl;
				cout << "-Press \u001b[33mEscape(ESC)\u001b[0m to exit View Product Mode and discard changes" << endl;
			}
			option = _getch();
			if (int(option) == -32) { //special char eg:arrow 
				option = _getch();
				switch (option) {
				case 72: //Up Arrow   
					if (intOpt > 0) {
						intOpt--;
					}
					else {
						intOpt = maxOpt;
					}
					break;
				case 80://down arrow    
					if (intOpt < maxOpt) {
						intOpt++;
					}
					else {
						intOpt = 0;
					}
					break;
				case 77: //right arrow 
					if (isEditing && !thisOrder.Orderitems.empty() && (intOpt >= 0 && intOpt < thisOrder.Orderitems.size())) {
						thisOrder.addProduct(thisOrder.Orderitems[intOpt]);
					}
					break;
				case 75://left arrow 
					if (isEditing && !thisOrder.Orderitems.empty() && (intOpt >= 0 && intOpt < thisOrder.Orderitems.size())) {
						thisOrder.removeProduct(thisOrder.Orderitems[intOpt]);
					}
					break;
				}
			}
			else {
				if (option == '\r') {
					if (!isEditing) {
						switch (intOpt) {
						case 0:
							thisOrder.InsertNew();
							ManageOrder(user);
							MainMenu(user);
							break;
						case 1:
							isEditing = true;
							intOpt = 0;
							break;
						case 2:
							return false;
							break;
						case 3:
							order->Orderitems.clear();
							thisOrder.Orderitems.clear();
							return false;
						default:
							break;
						}
					}
					else {
						order->Orderitems = thisOrder.Orderitems;
						isEditing = false;
						intOpt = 0;
					}
				}
				else if (option == 27) {
					if (isEditing) {
						intOpt = 0;
						thisOrder = *order;
						isEditing = false;
					}
				}
			}
		}
		else if (thisOrder.Status == 0) { //new ==================
			cout << endl;
			if (intOpt == 0 || isEditing)
				cout << "\u001b[33m>";
			cout << ">View Product Detail" << "\u001b[0m" << endl;
			if (thisOrder.CustomerIC == user->IC) {//user is the customer
				isVendor = false;
				if (intOpt == 1 && !isEditing)
					cout << "\u001b[33m>";
				cout << ">Cancel Order" << "\u001b[0m" << endl;
			}
			else {//user is not customer aka vendor
				isVendor = true;
				if (intOpt == 1 && !isEditing)
					cout << "\u001b[33m>";
				cout << ">Accept Order" << "\u001b[0m" << endl;
				if (intOpt == 2 && !isEditing)
					cout << "\u001b[33m>";
				cout << ">Refuse Order" << "\u001b[0m" << endl;
			}
			if (!isEditing) {
				if (isVendor)
					maxOpt = 3;
				else
					maxOpt = 2;
			}
			if (intOpt == maxOpt && !isEditing)
				cout << "\u001b[33m>";
			cout << ">Back" << "\u001b[0m" << endl;

			if (!isEditing) {
				displayMenuGuide();
			}
			else {
				cout << endl << "--View Product Mode--" << endl << "-Press \u001b[33mUp/Down\u001b[0m Arrow key to select a product"
					<< endl << "-Press \u001b[33mEnter\u001b[0m to view product detail" << endl;
				cout << "-Press \u001b[33mEscape(ESC)\u001b[0m to exit View Product Mode" << endl;
			}
			option = _getch();

			if (int(option) == -32) { //special char eg:arrow 
				option = _getch();
				switch (option) {
				case 72: //Up Arrow   
					if (intOpt > 0) {
						intOpt--;
					}
					else {
						intOpt = maxOpt;
					}
					break;
				case 80://down arrow    
					if (intOpt < maxOpt) {
						intOpt++;
					}
					else {
						intOpt = 0;
					}
					break;
				}
			}
			else {
				if (option == '\r') {
					switch (intOpt) {
					case 0:
						if (!isEditing) {
							isEditing = true;

							break;
						}
					case 1:
						//cancel or accpet  
						if (!isEditing) {
							system("cls");
							if (isVendor)
								cout << "Are you sure you want to accept this order ?";
							else
								cout << "Are you sure you want to cancel this order ?";
							cout << endl << "Press ENTER to proceed / any key to go back";
							confirm = _getch();
							if (confirm == '\r') {
								changes = true;
								if (isVendor) {
									thisOrder.Status = 1;
									thisOrder.updateStatus(1);
								}
								else
								{
									thisOrder.Status = 3;
									thisOrder.updateStatus(3);
								}
							}
							break;
						}
					case 2:
						//refuse for vendor, if is customer go back
						if (!isEditing) {
							if (isVendor) {
								system("cls");
								cout << "Are you sure you want to refuse this order ?";
								cout << endl << "Press ENTER to proceed / any key to go back";
								confirm = _getch();
								if (confirm == '\r') {
									changes = true;
									thisOrder.Status = 4;
									thisOrder.updateStatus(4);
								}
							}
							else {
								return changes;
							}
							break;
						}
					case 3:
						if (!isEditing && isVendor) {
							return changes;
							break;
						}
					default:
						if (isEditing) {
							selectedOrderProduct = Product(thisOrder.Orderitems[intOpt].ProductId);
							selectedorderShop = Shop(thisOrder.ShopId);
							ProductDetails(user, &selectedorderShop, &selectedOrderProduct, true);
						}
						//	if(isEditing)
								//thisorder[intopt] go detail
						break;
					}

				}
				else if (option == 27) {
					if (isEditing) {
						isEditing = false;
						intOpt = 0;
					}
					else
						return changes;
				}
			}

		}
		else if (thisOrder.Status == 1 && isVendor) { //accepted, can update to ready by vendor ==========
			cout << endl;
			if (intOpt == 0 || isEditing)
				cout << "\u001b[33m>";
			cout << ">View Product Detail" << "\u001b[0m" << endl;
			isVendor = true;
			if (intOpt == 1 && !isEditing)
				cout << "\u001b[33m>";
			cout << ">Update Ready" << "\u001b[0m" << endl;
			if (!isEditing) {
				maxOpt = 2;
			}
			if (intOpt == maxOpt && !isEditing)
				cout << "\u001b[33m>";
			cout << ">Back" << "\u001b[0m" << endl;


			if (!isEditing) {
				displayMenuGuide();
			}
			else {
				cout << endl << "--View Product Mode--" << endl << "-Press \u001b[33mUp/Down\u001b[0m Arrow key to select a product"
					<< endl << "-Press \u001b[33mEnter\u001b[0m to view product detail" << endl;
				cout << "-Press \u001b[33mEscape(ESC)\u001b[0m to exit View Product Mode" << endl;
			}

			option = _getch();

			if (int(option) == -32) { //special char eg:arrow 
				option = _getch();
				switch (option) {
				case 72: //Up Arrow   
					if (intOpt > 0) {
						intOpt--;
					}
					else {
						intOpt = maxOpt;
					}
					break;
				case 80://down arrow    
					if (intOpt < maxOpt) {
						intOpt++;
					}
					else {
						intOpt = 0;
					}
					break;
				}
			}
			else {
				if (option == '\r') {
					switch (intOpt) {
					case 0:
						if (!isEditing) {
							isEditing = true;
							break;
						}
					case 1:
						if (!isEditing) {
							system("cls");
							cout << "Are you sure the order is ready ?";
							cout << endl << "Press ENTER to confirm / any key to go back";
							confirm = _getch();
							if (confirm == '\r') {
								changes = true;
								thisOrder.Status = 2;
								thisOrder.updateStatus(2);
							}
							break;
						}
					case 2:
						if (!isEditing) {
							return changes;
							break;
						}
					default:
						if (isEditing) {
							selectedOrderProduct = Product(thisOrder.Orderitems[intOpt].ProductId);
							selectedorderShop = Shop(thisOrder.ShopId);
							ProductDetails(user, &selectedorderShop, &selectedOrderProduct, true);
						}
						break;
					}

				}
				else if (option == 27) {
					if (isEditing) {
						isEditing = false;
						intOpt = 0;
					}
					else
						return changes;
				}
			}
		}
		else if ((thisOrder.Status == 2 || thisOrder.Status == 7) && isVendor) { //2 ready 7 pendingcan go complete or failed
			cout << endl;
			if (intOpt == 0 || isEditing)
				cout << "\u001b[33m>";
			cout << ">View Product Detail" << "\u001b[0m" << endl;
			isVendor = true;
			if (intOpt == 1 && !isEditing)
				cout << "\u001b[33m>";
			if (thisOrder.Status == 7)
				cout << ">Update payment recieved" << "\u001b[0m" << endl;
			else
				cout << ">Confirm payment recieved" << "\u001b[0m" << endl;
			if (intOpt == 2 && !isEditing)
				cout << "\u001b[33m>";
			cout << ">Mark as failed" << "\u001b[0m" << endl;
			if (!isEditing) {
				maxOpt = 3;
			}
			if (intOpt == maxOpt && !isEditing)
				cout << "\u001b[33m>";
			cout << ">Back" << "\u001b[0m" << endl;


			if (!isEditing) {
				displayMenuGuide();
			}
			else {
				cout << endl << "--View Product Mode--" << endl << "-Press \u001b[33mUp/Down\u001b[0m Arrow key to select a product"
					<< endl << "-Press \u001b[33mEnter\u001b[0m to view product detail" << endl;
				cout << "-Press \u001b[33mEscape(ESC)\u001b[0m to exit View Product Mode" << endl;
			}

			option = _getch();

			if (int(option) == -32) { //special char eg:arrow 
				option = _getch();
				switch (option) {
				case 72: //Up Arrow   
					if (intOpt > 0) {
						intOpt--;
					}
					else {
						intOpt = maxOpt;
					}
					break;
				case 80://down arrow    
					if (intOpt < maxOpt) {
						intOpt++;
					}
					else {
						intOpt = 0;
					}
					break;
				}
			}
			else {
				if (option == '\r') {
					switch (intOpt) {
					case 0:
						if (!isEditing) {
							isEditing = true;
							break;
						}
					case 1:
						if (!isEditing) { //confirm payment later prompt by amount 
							cout << endl << "Please enter the payment you recieved to update/confirm payment" << endl;
							set_cursor(true);
							cin >> payment;
							set_cursor(false);
							if (cin.fail()) {
								cin.clear();
								cin.ignore(1000, '\n');
								cout << "\u001b[41;1m\u001b[37minvalid input, process failed Please try again\u001b[0m";
								_getch();
							}
							else {
								if (thisOrder.Status == 7) {
									payment = payment + thisOrder.PaymentRecieved;
								}
								if (payment < thisOrder.TotalPrice) {
									cout << "\u001b[41;1m\u001b[37m Payment recieved is less than total price of the order, Do you still want to proceed? \u001b[0m";
								}
								cout << endl << "Press ENTER to confirm / any key to go back" << endl << "NOTE: After payment is confirmed no further changes is allowed on the other";
								confirm = _getch();
								if (confirm == '\r') {
									changes = true;
									if (payment < thisOrder.TotalPrice) {
										thisOrder.Status = 7;
										thisOrder.PaymentRecieved = payment;
										thisOrder.UpdatePendingPayment(payment);
									}
									else {
										thisOrder.Status = 6;
										thisOrder.CompleteDate = strDateTimeNow();
										thisOrder.CompleteOrder(payment, strDateTimeNow());
									}


								}
							}
							break;
						}
					case 2:
						if (!isEditing) {
							system("cls");
							cout << "Mark the order as failed? \nPlease state your reason(for example= customer didn't pick up for a long time)\n100 letter max:" << endl;
							set_cursor(true);
							comment = "";
							comment = getchLine(100, 1, &comment);
							set_cursor(false);
							cout << endl << "Press ENTER to confirm / any key to go back";
							confirm = _getch();
							if (confirm == '\r') {
								changes = true;
								thisOrder.Status = 5;
								thisOrder.Comment += "<FV>" + comment + "<FV>";
								thisOrder.FailByVendorComment(comment);
							}
							break;
						}
					case 3:
						if (!isEditing) {
							return changes;
							break;
						}
					default:
						if (isEditing) {
							selectedOrderProduct = Product(thisOrder.Orderitems[intOpt].ProductId);
							selectedorderShop = Shop(thisOrder.ShopId);
							ProductDetails(user, &selectedorderShop, &selectedOrderProduct, true);
						}
						break;
					}

				}
				else if (option == 27) {
					if (isEditing) {
						isEditing = false;
						intOpt = 0;
					}
					else
						return changes;
				}
			}
		}
		else { //view only 
			printLine('_');

			cout << endl << "--View Product Mode--" << endl << "-Press \u001b[33mUp/Down\u001b[0m Arrow key to select a product"
				<< endl << "-Press \u001b[33mEnter\u001b[0m to view product detail" << endl;
			cout << "-Press \u001b[33mEscape(ESC)\u001b[0m to go back" << endl;
			isEditing = true;
			option = _getch();
			if (int(option) == -32) { //special char eg:arrow 
				option = _getch();
				switch (option) {
				case 72: //Up Arrow   
					if (intOpt > 0) {
						intOpt--;
					}
					else {
						intOpt = maxOpt;
					}
					break;
				case 80://down arrow    
					if (intOpt < maxOpt) {
						intOpt++;
					}
					else {
						intOpt = 0;
					}
					break;
				}
			}
			else {
				if (option == '\r') {
					selectedOrderProduct = Product(thisOrder.Orderitems[intOpt].ProductId);
					selectedorderShop = Shop(thisOrder.ShopId);
					ProductDetails(user, &selectedorderShop, &selectedOrderProduct, true);
				}
				else if (option == 27) {
					return changes;
				}
			}

		}

	} while (option != '\0');
	return false;
}

void ManageOrder(Account* user, Shop* myShop) {
	ChangeScreen("Manage Order");
	cout << "Your Order History :" << endl;
	printLine('=');
	Orders findOrders;
	vector<Orders> orderlist;
	string keyShopName;
	string keyIC;
	char c = ' ';
	int selected = 0;
	bool isEditing = false;
	bool searchChange = true;
	int sortBy = 0;
	bool ASC = true;
	bool forwardOption = false;
	bool isVendor = false;
	string sortBycategory[] = { "Date","Total Price" };
	string status[] = { "New", "Accepted" , "Ready" ,"Canceled", "Refused" , "Failed" , "Completed" , "Pending Payment" };
	string statusFilter[] = { "All", "New", "Accepted" , "Ready" ,"Canceled", "Refused" , "Failed" , "Completed" , "Pending Payment" };
	int intStatusFilter = 0;

	if (myShop != NULL) { //vendor
		findOrders.ShopId = myShop->ShopId;
		isVendor = true;
	}
	else {//customer
		findOrders.CustomerIC = user->IC;
	}
	do {
		system("cls");
		//db op 
		if (searchChange) {
			if (isVendor)
				findOrders.CustomerIC = keyIC;
			else
				findOrders.ShopName = keyShopName;
			if (sortBy == 0)
				orderlist = findOrders.getOrderSortByDate(ASC, intStatusFilter - 1);
			else if (sortBy == 1)
				orderlist = findOrders.getOrderSortByTotalPrice(ASC, intStatusFilter - 1);
			searchChange = false;
			selected = 0;
		}

		//prompt
		if (isVendor)
			cout << "\033[4m\u001b[36m" << myShop->ShopName << " order history\033[0m" << endl;
		else
			cout << "\033[4mYour order history\033[0m" << endl;
		if (isEditing)
			cout << "\u001b[33m>>";
		if (!isVendor)
			cout << "Filter by Shop Name : " << keyShopName << "\u001b[0m" << endl;
		if (isVendor)
			cout << "Filter by Customer IC : " << keyIC << "\u001b[0m" << endl;
		cout << "Display : " << statusFilter[intStatusFilter] << endl;
		cout << "[ Found : " << orderlist.size() << " ][SortBy:" << sortBycategory[sortBy] << "][Order:";
		if (ASC) {
			if (sortBy == 0)
				cout << "Oldest -> Latest";
			else
				cout << "Ascending";
		}
		else {
			if (sortBy == 0)
				cout << "Latest -> Oldest";
			else
				cout << "Descending";
		}
		cout << "]" << endl;
		cout << "Press ";


		printLine('=');
		if (!orderlist.empty()) {
			for (int i = 0; i < orderlist.size(); i++) {
				if ((selected - i <= 2 && selected - i >= -2)
					|| (selected == 1 && (selected - i <= 1 && selected - i >= -3))
					|| (selected == 0 && (i - selected < 5 && i - selected >= 0))
					|| (selected == orderlist.size() - 2 && (selected - i <= 3 && selected - i >= -1))
					|| (selected == orderlist.size() - 1 && (selected - i >= 0 && selected - i < 5))) {

					if (i == selected)
						cout << "\u001b[36m";

					printLine('_');
					cout << endl << "Order Id : " << orderlist[i].OrderId << " [ Order Date : " << orderlist[i].OrderDate << " ]" << endl;
					if (isVendor)
						cout << "Shop Name : " << orderlist[i].ShopName << endl;
					else
						cout << "[Shop Name: " << orderlist[i].ShopName << "| Contact No.:" << orderlist[i].CustContactNo << "]" << endl;
					if (isVendor)
						cout << "[Customer IC: " << orderlist[i].CustomerIC << "| Name: " << orderlist[i].CustName << "| Contact No.: " << orderlist[i].CustContactNo << "]" << endl;
					else
						cout << "Cutomer IC: " << orderlist[i].CustomerIC << endl;
					cout << "Total Price : RM "; printf("%.2f", orderlist[i].TotalPrice); cout << endl;
					cout << "Status : [ ";
					if (orderlist[i].Status == 1)
						cout << "\u001b[36m";
					else if (orderlist[i].Status == 2)
						cout << "\u001b[32m";
					else if (orderlist[i].Status >= 3 && orderlist[i].Status <= 5)
						cout << "\u001b[31m";
					else if (orderlist[i].Status == 6)
						cout << "\u001b[33m";
					else if (orderlist[i].Status == 7)
						cout << "\u001b[34m";

					cout << status[orderlist[i].Status];
					if (orderlist[i].Status != 0)
						cout << "\u001b[0m";

					if (i == selected)
						cout << "\u001b[36m";
					cout << " ]";
					printLine('_');

					if (i == selected)
						cout << "\u001b[0m";
				}
			}
		}


		cout << endl << endl << "\u001b[33m====GUIDE====\u001b[0m" << endl;
		cout << "--Press \u001b[33mUp/Down arrow\u001b[0m key to select an order" << endl
			<< "--Press \u001b[33mLeft/Right arrow\u001b[0m key to filter result by order status" << endl
			<< "--Press \u001b[33mEnter\u001b[0m to go to order details" << endl;
		if (isEditing) {
			cout << "--Press \u001b[33mBackspace\u001b[0m to delete a character from the search filter" << endl;
			cout << "--Press \u001b[33mEscape(ESC)\u001b[0m to stop searching" << endl;
		}
		else {
			if (isVendor) {
				cout << "--Press \u001b[33m1\u001b[0m to search by Customer IC" << endl;
			}
			else {
				cout << "--Press \u001b[33m1\u001b[0m to search by Shop Name" << endl;
			}
			cout << "--Press \u001b[33m2\u001b[0m to change sorting category" << endl;
			cout << "--Press \u001b[33m3\u001b[0m to change sorting order" << endl;
			cout << "--Press \u001b[33mEscape(ESC)\u001b[0m to go back" << endl;
		}


		if (!forwardOption) {
			c = _getch();
		}
		else {
			forwardOption = false;
		}

		if (int(c) == -32) { //special char eg:arrow 
			c = _getch();
			switch (c) {
			case 72: //Up Arrow   
				if (selected > 0) {
					selected--;
				}
				break;
			case 80://down arrow    
				if (selected < orderlist.size() - 1) {
					selected++;
				}
				break;
			case 77: //right arrow 
				if (intStatusFilter < 8) {
					intStatusFilter++;
					searchChange = true;
				}
				break;
			case 75://left arrow 
				if (intStatusFilter > 0) {
					intStatusFilter--;
					searchChange = true;
				}
				break;
			}
		}
		else {
			switch (c) {
			case 72: //Up Arrow   
				if (selected > 0) {
					selected--;
				}
				break;
			case 80://down arrow    
				if (selected < orderlist.size() - 1) {
					selected++;
				}
				break;
			case 77: //right arrow 
				if (intStatusFilter < 8) {
					intStatusFilter++;
					searchChange = true;
				}
				break;
			case 75://left arrow 
				if (intStatusFilter > 0) {
					intStatusFilter--;
					searchChange = true;
				}
				break;
			case '\r':
				if (isEditing) {
					isEditing = false;
				}
				else if (!orderlist.empty() && selected >= 0 && selected < orderlist.size()) {
					//open detail
					Orders selectedOrder(orderlist[selected].OrderId);//call constructor which use id and load include order items
					searchChange = OrderDetails(user, &selectedOrder);
				}
				break;
			case '\b':
				if (!isVendor && isEditing && keyShopName.size() > 0) {
					keyShopName.resize(keyShopName.size() - 1);
					searchChange = true;
				}
				if (isVendor && isEditing && keyIC.size() > 0) {
					keyIC.resize(keyIC.size() - 1);
					searchChange = true;
				}
				break;
			case 27:
				if (isEditing)
					isEditing = false;
				else
					return;
				break;
			case '1':
				if (!isEditing) {
					isEditing = true;
					break;
				}
			case '2':
				if (!isEditing) {
					if (sortBy == 1)
						sortBy = 0;
					else
						sortBy = 1;
					searchChange = true;
					break;
				}
			case '3':
				if (!isEditing) {
					if (ASC)
						ASC = false;
					else
						ASC = true;
					searchChange = true;
					break;
				}
			default:
				if (isEditing) {
					if (isVendor) {
						if ((int(c) >= 48) && (int(c) <= 57)) {
							if (keyIC.size() < 12) {
								keyIC += c;
								searchChange = true;
							}
						}
					}
					else if ((int(c) >= 65 && int(c) <= 90) || (int(c) >= 97 && int(c) <= 122) || int(c) == 32 || int(c) == 44 || int(c) == 46 || (int(c) >= 48 && int(c) <= 57)) {
						//varchars char + num + sum special char                                  space          comma            dot               numbers
						if (keyShopName.size() < 50) {
							keyShopName += c;
							searchChange = true;
						}
					}
				}
				else {
					searchChange = false; //if not inserting aka still in find shop main window but user insert unnecessary char, no change so no ned to requery at top
					do {
						c = _getch();
						if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '\r' || c == 27) {
							break;
						}
						if (int(c) == -32) {
							c = _getch();
							if (c == 72 || c == 80) {
								break;
							}
						}
					} while (c != '\0');
					forwardOption = true;
				}
				break;
			}
		}
	} while (1);

}


void SaleAnalysisMenu(Account* user, Shop* myShop) {
	ChangeScreen("Sale Analysis of " + myShop->ShopName);
	system("cls");
	char option = ' ';
	int intOpt = 0;
	int maxOpt = 0;
	int year = 0;
	int yearStart = parseDateToYear(myShop->RegisterDate);
	int yearNow = parseDateToYear(strDateTimeNow());
	int MonthStart = parseDateToMonth(myShop->RegisterDate);
	int MonthNow = parseDateToMonth(strDateTimeNow());
	int yearSelected = 0;
	bool isSelectAnalysis = false;
	bool allowSemiAnnual = false;
	bool allowAnnual = false;
	SaleAnalysis sale(myShop->ShopId);
	if (yearStart == yearNow && (MonthNow - MonthStart) < 5) {
		cout << "You need to at least operates for 6 month to access this feature" << endl
			<< "Your shop " << myShop->ShopName << " has only operated for " << MonthNow - MonthStart << " Month, Please come back later"
			<< endl << "Press any key to go back" << endl;
		_getch();
		return;
	}
	if (MonthNow < 6) {//to little month this year so no report yet
		yearNow = yearNow - 1;
	}
	vector<int> availableYear;
	for (int i = yearStart; i <= yearNow; i++) {
		availableYear.push_back(i);
	}

	while (1) {
		system("cls");
		cout << "===== Sale Analysis options =====" << endl;
		if (!isSelectAnalysis) {
			cout << "Choose a year to access sale analysis" << endl;

			for (int i = 0; i < availableYear.size(); i++) {
				if (intOpt == i)
					cout << "\u001b[33m>";
				cout << ">Year " << availableYear[i] << "\u001b[0m" << endl;
			}
			maxOpt = availableYear.size();
		}
		else {
			cout << "Select sale analysis to generate" << endl;
			maxOpt = 1;
			if (intOpt == 0)
				cout << "\u001b[33m>";
			cout << ">Quarterly Sale Analysis\u001b[0m" << endl;

			if (allowSemiAnnual) {
				maxOpt = 2;
				if (intOpt == 1)
					cout << "\u001b[33m>";
				cout << ">Semiannual Sale Analysis\u001b[0m" << endl;
			}
			if (allowAnnual) {
				maxOpt = 3;
				if (intOpt == 2)
					cout << "\u001b[33m>";
				cout << ">Annual Sale Analysis\u001b[0m" << endl;
			}
		}

		displayMenuGuide();
		option = _getch();

		if (int(option) == -32) { //special char eg:arrow 
			option = _getch();
			switch (option) {
			case 72: //Up Arrow   
				if (intOpt > 0) {
					intOpt--;
				}
				break;
			case 80://down arrow    
				if (intOpt < maxOpt - 1) {
					intOpt++;
				}
				break;
			}
		}
		else {
			if (option == '\r') {
				if (isSelectAnalysis) {
					if (intOpt == 0) {//quarter 
						system("cls");
						sale.doQuarterAnalysis(yearSelected);
					}
					else if (intOpt == 1 && allowSemiAnnual) {
						system("cls");
						sale.doSemiAnnualAnalysis(yearSelected); 
					}
					else if (intOpt == 2 && allowAnnual) {
						system("cls");
						sale.doAnnualAnalysis(yearSelected);
					}
				}
				else if (!isSelectAnalysis && (!availableYear.empty() && intOpt < availableYear.size())) {
					//if year is now month less than 9 quarter only else quarter and semi annual, quarter at least 6 month if year less than no issue
					isSelectAnalysis = true;
					yearSelected = availableYear[intOpt];
					intOpt = 0;
					if (yearSelected != parseDateToYear(strDateTimeNow())) {
						allowAnnual = true;
					}
					if (yearSelected == parseDateToYear(strDateTimeNow())) {
						if (MonthNow >= 9) {
							allowSemiAnnual = true;
						}
						if (MonthNow >= 11) {
							allowAnnual = true;
						}
					}
					else {
						allowSemiAnnual = true;
					}

				}
			}
			else if (option == 27) {
				if (isSelectAnalysis) {
					isSelectAnalysis = false;
					intOpt = 0;
				}
				else
					return;
			}
		}
	}
}

int parseDateToMonth(string date) {
	int s = date.find("-");
	int e = date.find_last_of("-");
	return stoi(date.substr(s + 1, e));
}
int parseDateToYear(string date) {

	std::size_t found = date.find("-");
	date = date.substr(0, found);

	return stoi(date);
}

string ParseComment(string comment) {
	string strReturn;
	int s;
	int e;
	std::size_t found = comment.find("<FV>");
	if (found != std::string::npos) { //found
		s = found + 4;
		found = comment.find_last_of("<FV>");
		e = found - 7;
		strReturn += "\nMarked failed by vendor, Reason:\n"
			+ comment.substr(s, e);

	}
	return strReturn;
}

void displayMenuGuide(bool editable) {
	if (editable)
	{
		cout << endl << "====Guide====";
		cout << endl << "---Press \u001b[33mUp/Down arrow\u001b[0m key to select a field/option" << endl << "---Press \u001b[33mEnter\u001b[0m after selecting to edit/proceed"
			<< endl << "---Press \u001b[33mEscape(ESC)\u001b[0m to navigate to previous menu/Cancel input";
	}
	else {
		cout << endl << "====Guide====";
		cout << endl << "---Press \u001b[33mUp/Down arrow\u001b[0m key to select an option in the menu" << endl << "---Press \u001b[33mEnter\u001b[0m after selecting an option to proceed"
			<< endl << "---Press \u001b[33mEscape(ESC)\u001b[0m to navigate to previous menu";
	}
}

void displayInsertGuide(int max, int mode) {
	cout << endl << endl << "====INSERTING/EDITING GUIDE====";
	if (mode == 1)
		cout << endl << "---Press any \u001b[33malphabet/number/space\u001b[0m key to fill the data";
	if (mode == 2)
		cout << endl << "---Press any \u001b[33malphabet/space\u001b[0m key to fill the data";
	if (mode == 3)
		cout << endl << "---Press any \u001b[33mnumber\u001b[0m key to fill the data";
	cout << endl << "---Press \u001b[33mTab/Enter\u001b[0m to finish inserting" << endl << "---Press \u001b[33mEscape(ESC)\u001b[0m to cancel/reset insert"
		<< endl << "---Press \u001b[33mRight/Left\u001b[0m Arrow key to move the caret" << endl << "---Press \u001b[33mBackspace\u001b[0m to delete one character"
		<< endl << "---The insert will automatically finished when input reached the \u001b[33mmaximum length : " << max << "\u001b[0m character";
	if (mode != 3)
		cout << endl << "---Any space at the beginning and the end of input will be deleted upon finish inserting";
}

//non module function

int LinerSearchProduct(vector<Product> * p, Product * key) {
	for (int i = 0; i < p->size(); i++) {
		if (p->at(i).ProductId == key->ProductId)
			return i;
	}
	return -1;
}

void ChangeScreen(string title) {
	system("cls");
	SetConsoleTitleA(("Frozen Food Ordering System: " + title).c_str());
}

string strPlaceHolder(string text, char placeHolder) {
	string txt;
	for (int i = 0; i < text.size(); i++) {
		txt += placeHolder;
	}
	return txt;
}

string getchLine(int max, int mode, string* curString, int dPlace) { //mode 1=varchar, 2=CharOnly, 3=numberOnly, 4=decimal
	int i = 0;
	int line = 1;
	int curLine = 1;
	string FullString;
	COORD tmp, consoleSize;
	consoleSize = getConsoleSize();
	if (curString->size() > 0) {
		FullString = *curString;
		int tmpSize = max;
		while (tmpSize > consoleSize.X) {
			tmpSize = tmpSize - consoleSize.X;
			line++;
		}
	}
	char c;
	while (i < max) {
		if (i > consoleSize.X * curLine) {
			cout << endl;
			curLine++;
			line++;
		}
		if (i != 0 && (i == consoleSize.X * curLine - consoleSize.X)) {
			tmp = getCursorPos();
			MoveCursor(consoleSize.X, tmp.Y - 1);
			if (c = '\b') {
				cout << FullString[i];
			}
			if (i < FullString.size()) {
				MoveCursor(0, tmp.Y);
				cout << FullString.substr(i + 1) << " ";
				MoveCursor(consoleSize.X, tmp.Y - 1);
			}
			else {
				line--;
			}
			curLine--;
		}
		fflush(stdin);
		c = _getch();
		if (int(c) == -32) {
			c = _getch();
			switch (c) {
			case 77: //Right Arrow
				if (i < FullString.length()) {
					tmp = getCursorPos();
					MoveCursor(tmp.X + 1, tmp.Y);
					i++;
				}
				break;
			case 75://left arrow
				if (i > 0) {
					cout << '\b';
					i--;
				}
				break;
			case 72://up arrow
				if (curLine > 1 && i > consoleSize.X) {
					tmp = getCursorPos();
					MoveCursor(tmp.X, tmp.Y - 1);
					i = i - consoleSize.X - 1;
					curLine--;
				}
				break;
			case 80://down arrow go to next line if exist
				if (curLine < line && i < consoleSize.X * curLine) {
					cout << endl;
					i = consoleSize.X * curLine + 1;
					curLine++;
				}
				break;
			default:
				break;
			}
		}
		else {
			switch (c) {
			case '\t':
			case '\r':
				if (FullString.size() > max) {
					FullString.resize(max);
				}
				FullString.erase(std::find(FullString.begin(), FullString.end(), '\0'), FullString.end());
				strTrimSpace(&FullString);
				return FullString;
				break;
			case 27:
				return *curString;
				break;

			case '\b':
				if (i > 0 && i < FullString.length()) {
					/*
					cout << " \b";
					tmp = getCursorPos();
					cout << FullString.substr(i + 1) + " ";
					FullString = FullString.substr(0, i) + FullString.substr(i + 1);

					if (i > 0) {
						i--;
						tmp.X = tmp.X - 1;
					}

					MoveCursor(tmp.X, tmp.Y);*/

					cout << "\b";
					tmp = getCursorPos();
					FullString = FullString.substr(0, i - 1) + FullString.substr(i);
					cout << FullString.substr(i - 1) << " ";
					i--;
					MoveCursor(tmp.X, tmp.Y);
				}
				else if (i > 0 && i >= FullString.length()) {
					cout << "\b \b";
					i--;
					FullString.resize(i);
				}
				break;
			default:																									//space		 comma				dot
				if ((mode == 2 || mode == 1) && ((int(c) >= 65 && int(c) <= 90) || (int(c) >= 97 && int(c) <= 122) || int(c) == 32 || int(c) == 44 || int(c) == 46)) {
					FullString.resize(max - 1);
					cout << c;

					if (i < max) {
						//	FullString.resize(max-1);
						tmp = getCursorPos();
						cout << FullString.substr(i);
						MoveCursor(tmp.X, tmp.Y);
					}
					FullString.insert(i, 1, c);
					i++;
				}
				if (((mode == 3 || mode == 1 || mode == 4) && int(c) >= 48 && int(c) <= 57) || (mode == 4 && int(c) == 46)) {
					cout << c;
					if (mode == 1) {
						if (i < max) {
							tmp = getCursorPos();
							cout << FullString.substr(i);
							MoveCursor(tmp.X, tmp.Y);
						}
						FullString.insert(i, 1, c);
					}
					else {
						if (mode == 4 && int(c) == 46) {
							if (i == 0) {
								return *curString;
							}
							if (FullString.find('.') != std::string::npos) { //already has dot -> exit
								if (FullString.find('.') >= i) {
									FullString.erase(i + 1);
									max = i + 1 + dPlace;
								}
								else {
									return *curString;
								}

							}
							else {
								max = i + 1 + dPlace;
							}
						}
						FullString.replace(i, 1, 1, c);
					}
					i++;
				}
			}

		}
	}
	if (FullString.size() > max) {
		FullString.resize(max);
	}
	FullString.erase(std::find(FullString.begin(), FullString.end(), '\0'), FullString.end()); //trim null which created on resize()
	strTrimSpace(&FullString);
	return FullString;
}

void MoveCursor(int x, int y) {
	//initialize objects for cursor manipulation

	HANDLE hStdout;
	COORD destCoord;
	CONSOLE_CURSOR_INFO info;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	destCoord.X = x;
	destCoord.Y = y;
	SetConsoleCursorPosition(hStdout, destCoord);
}

void set_cursor(bool visible) {
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = visible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

COORD getCursorPos() {
	HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

string strDateTimeNow() {
	struct tm tmpTime;
	time_t now = time(0);
	localtime_s(&tmpTime, &now);
	string strDateTime = to_string(tmpTime.tm_year + 1900) + "-" + to_string(tmpTime.tm_mon + 1) + "-" + to_string(tmpTime.tm_mday) + " "
		+ to_string(tmpTime.tm_hour) + ":" + to_string(tmpTime.tm_min) + ":00";
	return strDateTime;
}

void printLine(char c) {
	COORD size = getConsoleSize();
	cout << "\n";
	for (int i = 0; i < size.X; i++) {
		cout << c;
	}
}

COORD getConsoleSize() {
	COORD size;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top;
	size.X = columns;
	size.Y = rows;
	return size;
}

void setConsoleSize(int width, int height) {
	HWND hwnd = GetConsoleWindow();

	RECT  rectWindow;
	GetWindowRect(hwnd, &rectWindow);
	int Midx, Midy;
	Midx = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	Midy = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	MoveWindow(hwnd, Midx, Midy, width, height, TRUE);
}

string strTrimSpaceEnd(string str) {
	int i = str.size() - 1;
	while (i > 0) {
		if (str.at(i) == ' ') {
			str.resize(i);
		}
		else {
			return str;
		}
		i--;
	}
	return str;
}

string strTrimSpaceStart(string str) {
	if (str[0] == ' ') {
		return strTrimSpaceStart(str.substr(1));
	}
	else {
		return str;
	}
	return str;
}

void strTrimSpace(string* str) {
	*str = strTrimSpaceStart(strTrimSpaceEnd(*str));
}

void generateVerticalSpace(int max) { // generate space based on max input size/buffer width 

	COORD tmp = getConsoleSize();
	cout << endl;
	while (max > tmp.X) {
		cout << endl;
		max = max - tmp.X;
	}
}

float roundByDecimalPlace(float num, int decimalPlace) {
	decimalPlace = pow(10, decimalPlace);
	num = round(num * decimalPlace);
	num = num / decimalPlace;
	return num;
}

float myStrToFloat(string* num) {
	if (num->empty()) return 0;
	if (num->at(num->size() - 1) == '.') {
		num->resize(num->size() - 1);
	}
	float fFloat;
	int i = num->find('.');
	if (i > 0 && i < num->size()) {
		fFloat = stoi(num->substr(0, i));
		fFloat = fFloat + (stoi(num->substr(i + 1)) / pow(10, num->substr(i + 1).size()));
	}
	else {
		fFloat = stoi(*num);
	}
	return fFloat;
}