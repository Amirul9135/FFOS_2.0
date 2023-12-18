#include <string>
#include "SaleAnalysis.h"
#include "DBConnection.h"
#include <mysql/jdbc.h> 
#include "gnuplot-iostream.h" 
#include <iomanip>
#include <conio.h>
#include <math.h>
#include <filesystem>
using namespace std;

SaleAnalysis::SaleAnalysis(int shopId) {
	ShopId = shopId;
	gnuPath = filesystem::current_path().string();
	gnuPath += "\\gnuplot\\bin\\gnuplot.exe";
	replaceAll(gnuPath, "\\", "\\\\");
	gnuPath = "\"" + gnuPath + "\"";  
}


void SaleAnalysis::replaceAll(string& str, const string& from, const string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}
//=======useful

 

void SaleAnalysis::doQuarterAnalysis(int year) {
	LoadQuartersFromDB_2(year);
	if (data01.empty()) {
		cout << endl << "No Data to be analyzed press any key to go back" << endl;
		_getch();
		return;
	}
	else {
		bool q2 = !data02.empty();
		bool q3 = !data03.empty();
		bool q4 = !data04.empty();
		double SaleQ1 = countSale(data01);
		cout << endl << "\033[4mQuarter Analysis for year " << year << " \033[0m" << endl;
		cout << "--First Quarter--\nTotal Sale(RM) : " << RoundDecimal(SaleQ1, 2);

		if (q2) {
			double SaleQ2 = countSale(data02);
			cout << endl << endl << "--Second Quarter--" << endl;
			cout << "Total Sale(RM) : "; printf("%.2f", SaleQ2); cout << endl; 
			if (SaleQ2 > SaleQ1) {
				cout << "Total Sale increased by \u001b[32m" << RoundDecimal((SaleQ2 / SaleQ1 * 100) - 100,2) << "%\u001b[0m From the first quarter";
			}
			else if (SaleQ2 < SaleQ1) {
				cout << "Total Sale decreased by \u001b[31m" << RoundDecimal((100 - (SaleQ2 / SaleQ1 * 100)),2) << "%\u001b[0m From the first quarter";
			}
			if (q3) {
				double SaleQ3 = countSale(data03);
				cout << endl << endl << "--Third Quarter--" << endl;
				cout << "Total Sale(RM) : "; printf("%.2f", SaleQ3); cout << endl;
				if (SaleQ3 > SaleQ2) {
					cout << "Total Sale increased by \u001b[32m" << RoundDecimal((SaleQ3 / SaleQ2 * 100) - 100, 2) << "%\u001b[0m From the second quarter";
				}
				else if (SaleQ3 < SaleQ2) {
					cout << "Total Sale decreased by \u001b[31m" << RoundDecimal((100 - (SaleQ3 / SaleQ2 * 100)), 2) << "%\u001b[0m From the second quarter";
				}
				if (q4) {
					double SaleQ4 = countSale(data04);
					cout << endl << endl << "--Fourth Quarter--" << endl;
					cout << "Total Sale(RM) : "; printf("%.2f", SaleQ4); cout << endl;
					if (SaleQ4 > SaleQ3) {
						cout << "Total Sale increased by \u001b[32m" << RoundDecimal((SaleQ4 / SaleQ3 * 100) - 100, 2) << "%\u001b[0m From the third quarter";
					}
					else if (SaleQ4 < SaleQ3) {
						cout << "Total Sale decreased by \u001b[31m" << RoundDecimal((100 - (SaleQ4 / SaleQ3 * 100)), 2) << "%\u001b[0m From the third quarter";
					}
				}
			}
		}
		cout << endl << endl << "--Press \u001b[33mUp/Down\u001b[0m Arrow key And \u001b[33mEnter\u001b[0m to choose an option below"<< endl ;
		int intOpt = 0;
		char option = ' ';
		while (1) {
			if (intOpt == 0)
				cout << "\u001b[33m>";
				cout  << ">Generate Total Sale/Month bar graph\u001b[0m" << endl;
			if (intOpt == 1)
				cout << "\u001b[33m>";
				cout << ">Generate Total Sale/Day Line graph\u001b[0m" << endl;
			if (intOpt == 2)
				cout << "\u001b[33m>";
			cout << ">End Analysis\u001b[0m";
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
			else if (option == '\r') {
				switch (intOpt) {
				case 0:
					generateQuarterComparisonBarGraph(year);
					break;
				case 1:
					generateQuarterComparisonLineGraph(year);
					break;
				case 2:
					return;
					break;
				}
			}
			cout << "\33[2K\033[A\33[2K\033[A\33[2K\r";//delete go up xr \r go to start col
		}  
	}

}

void SaleAnalysis::doSemiAnnualAnalysis(int year) {
	LoadSemiAnnualFromDB_2(year);
	if (data01.empty()) {
		cout << endl << "No Data to be analyzed  press any key to go back" << endl;
		_getch();
		return;
	}
	else {
		bool p2 = !data02.empty();
		double SaleP1 = countSale(data01);
		cout << endl << "\033[4mSemi Annual Analysis for year " << year << " \033[0m" << endl;
		cout << "--First Half--\nTotal Sale(RM) : " << RoundDecimal(SaleP1, 2);
		if (p2) {
			double SaleP2 = countSale(data02);
			cout << endl << endl << "--Second Half--" << endl;
			cout << "Total Sale(RM) : "; printf("%.2f", SaleP2); cout << endl;
			if (SaleP2 > SaleP1) {
				cout << "Total Sale increased by \u001b[32m" << RoundDecimal((SaleP2 / SaleP1 * 100) - 100, 2) << "%\u001b[0m From the first half";
			}
			else if (SaleP2 < SaleP1) {
				cout << "Total Sale decreased by \u001b[31m" << RoundDecimal((100 - (SaleP2 / SaleP1 * 100)), 2) << "%\u001b[0m From the first half";
			}
		}
		cout << endl << endl << "--Press \u001b[33mUp/Down\u001b[0m Arrow key And \u001b[33mEnter\u001b[0m to choose an option below" << endl;
		int intOpt = 0;
		char option = ' ';
		while (1) {
			if (intOpt == 0)
				cout << "\u001b[33m>";
			cout << ">Generate Total Sale/Month bar graph\u001b[0m" << endl;
			if (intOpt == 1)
				cout << "\u001b[33m>";
			cout << ">Generate Total Sale/Day Line graph\u001b[0m" << endl;
			if (intOpt == 2)
				cout << "\u001b[33m>";
			cout << ">End Analysis\u001b[0m";
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
			else if (option == '\r') {
				switch (intOpt) {
				case 0:
					generateSemiAnnualComparisonBarGraph(year);
					break;
				case 1:
					generateSemiAnnualComparisonLineGraph(year);
					break;
				case 2:
					return;
					break;
				}
			}
			cout << "\33[2K\033[A\33[2K\033[A\33[2K\r";//delete go up xr \r go to start col
		}

	}
}

void SaleAnalysis::doAnnualAnalysis(int year) {
	LoadAnnualFromDBByMonth(year);
	if (data01.empty()) {
		cout << endl << "No Data to be analyzed  press any key to go back" << endl;
		_getch();
		return;
	}
	else {
		LoadProductSoldCount(year);
		int maxPSold = 5;
		if(productSoldCount.size() < 5)
			maxPSold = productSoldCount.size();
		bool pYear = !data02.empty();
		double SaleCurYear = countSale(data01);
		cout << endl << "\033[4mAnnual Analysis for year " << year << " \033[0m" << endl;
		cout << "--" << year << "--\nTotal Sale(RM) : " << RoundDecimal(SaleCurYear, 2) << endl;
		cout << "Top 5 product by quantitiy sold:" << endl;
		for (int i = 0; i < maxPSold; i++) {
			cout << "(" << i + 1 << ") " << productSoldCount[i].first << ", " << productSoldCount[i].second << " Sold" << endl;
		}
		if (pYear) {
			LoadProductSoldCount(year -1);
			maxPSold = 5;
			if (productSoldCount.size() < 5)
				maxPSold = productSoldCount.size();
			double SalePYear = countSale(data02);
			cout << endl << endl << "--" << (year - 1) << "--" << endl;
			cout << "Total Sale(RM) : "; printf("%.2f", SalePYear); cout << endl;
			cout << "Top 5 product by quantitiy sold:" << endl;
			for (int i = 0; i < maxPSold; i++) {
				cout << "(" << i + 1 << ") " << productSoldCount[i].first << ", " << productSoldCount[i].second << " Sold" << endl;
			}
			cout << endl << endl << "----SUMMARY----" << endl;
			if (SaleCurYear > SalePYear) {
				cout << "Total Sale increased by \u001b[32m" 
					<< RoundDecimal((SaleCurYear / SalePYear * 100) - 100, 2) << "%\u001b[0m compared to " << (year - 1);
			}
			else if (SaleCurYear < SalePYear) {
				cout << "Total Sale decreased by \u001b[31m" 
					<< RoundDecimal((100 - (SaleCurYear / SalePYear * 100)), 2) << "%\u001b[0m compared to " << (year - 1);
			}
		}
		cout << endl << endl << "--Press \u001b[33mUp/Down\u001b[0m Arrow key And \u001b[33mEnter\u001b[0m to choose an option below" << endl;
		int intOpt = 0;
		char option = ' ';
		while (1) {
			if (intOpt == 0)
				cout << "\u001b[33m>";
			cout << ">Generate Total Sale/Month bar graph\u001b[0m" << endl;
			if (intOpt == 1)
				cout << "\u001b[33m>";
			cout << ">Generate Total Sale/Day Line graph\u001b[0m" << endl;
			if (intOpt == 2)
				cout << "\u001b[33m>";
			cout << ">End Analysis\u001b[0m";
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
			else if (option == '\r') {
				switch (intOpt) {
				case 0:
					generateAnnualComparisonBarGraph(year);
					break;
				case 1:
					generateAnnualComparisonLineGraph(year);
					break;
				case 2:
					return;
					break;
				}
			}
			cout << "\33[2K\033[A\33[2K\033[A\33[2K\r";//delete go up xr \r go to start col
		}

	}
}

void SaleAnalysis::generateAnnualComparisonBarGraph(int year) {
	LoadAnnualFromDBByMonth(year);
	data01.push_back(make_pair(13.13, countSale(data01)));
	data02.push_back(make_pair(12.87, countSale(data02)));
	Gnuplot gp(gnuPath);
	string startRange = "0";
	string endRange = "13.5";
	gp << "set term qt size 1600,900 title 'Sale Report Analysis'\n";
	gp << "set xrange ['" << startRange << "':'" << endRange << "']\n";
	gp << "set xtics (\"1st Month\" 1, \"2nd Month\" 2, \"3rd Month\" 3, \"4th Month\" 4,\"5th Month\" 5,\"6th Month\" 6,\"7th Month\" 7,\"8th Month\" 8,\"9th Month\" 9,\"10th Month\" 10,\"11th Month\" 11,\"12th Month\" 12,\"Whole Year\" 13) font \", 12\"\n";
	gp << "set title 'Annual analysis' font \"face, 20\"\n";
	gp << "set key font \"face, 14\"\n";
	gp << "set key spacing 1.3\n";
	gp << "set boxwidth 0.25\n";
	gp << "set ylabel \"Total Sale\" font \"face, 15\"\n";
	gp << "set style line 100 lt 1 lc rgb \"black\" lw 0.25\n";
	gp << "set grid ls 100\n";
	gp << "set key center top\n";
	gp << "set style fill solid 1.0\n";
	if (data01.size() > 1) {
		if (data02.size() > 1) {
			gp << "plot '-' using 1:2 with boxes title '" + to_string(year) + "',";
			gp << "'-'using 1:2 with boxes title '" + to_string(year - 1) + "'\n";
		}
		else {
			gp << "plot '-' using 1:2 with boxes title '" + to_string(year) + "'\n";
		}
		if (data01.size() > 1)
			gp.send1d(data01);
		if (data02.size() > 1)
			gp.send1d(data02);
	}
	else {
		cout << "No data to be analyzed";
		_getch();
		return;
	}
	_getch();
	return;
}
void SaleAnalysis::LoadAnnualFromDBByMonth(int year) {
	data01.clear();
	data02.clear(); 
	string strSqlQ1 = "SELECT DATE(CompleteDate) AS Date,TotalPrice FROM orders WHERE ShopId=" 
		+ to_string(ShopId) + " AND YEAR(CompleteDate)='" + to_string(year) + "' ORDER BY CompleteDate ASC";
	string strSqlQ2 = "SELECT DATE(CompleteDate) AS Date,TotalPrice FROM orders WHERE ShopId=" 
		+ to_string(ShopId) + " AND YEAR(CompleteDate)='" + to_string(year-1) + "' ORDER BY CompleteDate ASC";

	DBConnection db;
	db.QueryResult(strSqlQ1);
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (data01.size() > 0 && (data01.back().first == (dateStringToMonth(db.res->getString("Date")) + 0.13))) {
				data01.back().second = data01.back().second + db.res->getDouble("TotalPrice");
			}
			else {
				data01.push_back(std::make_pair(dateStringToMonth(db.res->getString("Date")) + 0.13, db.res->getDouble("TotalPrice")));
			}
		}
	}
	db.QueryResult(strSqlQ2);
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (data02.size() > 0 && data02.back().first == (dateStringToMonth(db.res->getString("Date")) - 0.13)) {
				data02.back().second = data02.back().second + db.res->getDouble("TotalPrice");
			}
			else {
				data02.push_back(std::make_pair(dateStringToMonth(db.res->getString("Date")) - 0.13, db.res->getDouble("TotalPrice")));
			}
		}
	}
	db.~DBConnection();
}

void SaleAnalysis::generateQuarterComparisonBarGraph(int year) {
	LoadQuartersFromDB_2(year);

	data01.push_back(make_pair(3.85, countSale(data01)));
	data02.push_back(make_pair(3.95, countSale(data02)));
	data03.push_back(make_pair(4.05, countSale(data03)));
	data04.push_back(make_pair(4.15, countSale(data04)));

	Gnuplot gp(gnuPath);


	string startRange = "0";
	string endRange = "4.5";
	gp << "set term qt size 1600,900 title 'Sale Report Analysis'\n";
	gp << "set xrange ['" << startRange << "':'" << endRange << "']\n";
	gp << "set xtics (\"1st Month\" 1, \"2nd Month\" 2, \"3rd Month\" 3, \"Total of all 3 Month\" 4) font \", 12\"\n";
	gp << "set title 'Quarterly Analysis " + to_string(year) + "' font \"face, 20\"\n"; 
	gp << "set boxwidth 0.1\n";
	gp << "set key font \"face, 12\"\n";
	gp << "set key spacing 1.3\n";
	gp << "set ylabel \"Total Sale\" font \"face, 15\"\n";
	gp << "set style line 100 lt 1 lc rgb \"black\" lw 0.25\n";
	gp << "set grid ls 100\n";
	gp << "set key center top\n"; 
	gp << "set style fill solid 1.0\n";
	if (data01.size() > 1) {
		if (data02.size() > 1) {
			gp << "plot '-' using 1:2 with boxes title 'First Quarter',";
			if (data03.size() > 1) {
				gp << "'-'using 1:2 with boxes title 'Second Quarter',";
				if (data04.size() > 1) {
					gp << "'-'using 1:2 with boxes title 'Third Quarter',";
					gp << "'-'using 1:2 with boxes title 'Fourth Quarter'\n";
				}
				else {
					gp << "'-'using 1:2 with boxes title 'Third Quarter'\n";
				}
			}
			else {
				gp << "'-'using 1:2 with lines boxes 'Second Quarter'\n";
			}
		}
		else {
			gp << "plot '-' using 1:2 with lines boxes 'First Quarter'\n";
		}
		if (data01.size() > 1)
			gp.send1d(data01);
		if (data02.size() > 1)
			gp.send1d(data02);
		if (data03.size() > 1)
			gp.send1d(data03);
		if (data04.size() > 1)
			gp.send1d(data04);
	}
	else {
		cout << "No data to be analyzed";
		_getch();
		return;
	}
	_getch();
	return;

}
void SaleAnalysis::LoadQuartersFromDB_2(int year) {
	data01.clear();
	data02.clear();
	data03.clear();
	data04.clear();
	string strSql = "SELECT DATE(CompleteDate) AS Date,TotalPrice FROM orders WHERE ShopId=" + to_string(ShopId) + " AND YEAR(CompleteDate)='" + to_string(year) + "' ";
	string strSqlQ1 = strSql + " AND MONTH(CompleteDate)>=1 AND MONTH(CompleteDate)<=3 ORDER BY CompleteDate ASC";
	string strSqlQ2 = strSql + " AND MONTH(CompleteDate)>=4 AND MONTH(CompleteDate)<=6 ORDER BY CompleteDate ASC";
	string strSqlQ3 = strSql + " AND MONTH(CompleteDate)>=7 AND MONTH(CompleteDate)<=9 ORDER BY CompleteDate ASC";
	string strSqlQ4 = strSql + " AND MONTH(CompleteDate)>=10 AND MONTH(CompleteDate)<=12 ORDER BY CompleteDate ASC";

	DBConnection db;
	db.QueryResult(strSqlQ1); 
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (data01.size() > 0 && (data01.back().first == (dateStringToMonth(db.res->getString("Date")) - 0.15))) {
				data01.back().second = data01.back().second + db.res->getDouble("TotalPrice");
			}
			else {
				data01.push_back(std::make_pair(dateStringToMonth(db.res->getString("Date")) - 0.15, db.res->getDouble("TotalPrice")));
			}
		}
	}

	db.QueryResult(strSqlQ2);
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (data02.size() > 0 && data02.back().first == (dateStringToMonth(db.res->getString("Date"), 3) - 0.05)) {
				data02.back().second = data02.back().second + db.res->getDouble("TotalPrice");
			}
			else {
				data02.push_back(std::make_pair(dateStringToMonth(db.res->getString("Date"), 3) - 0.05, db.res->getDouble("TotalPrice")));
			}
		}
	}


	db.QueryResult(strSqlQ3);
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (data03.size() > 0 && data03.back().first == (dateStringToMonth(db.res->getString("Date"), 6) + 0.05)) {
				data03.back().second = data03.back().second + db.res->getDouble("TotalPrice");
			}
			else {
				data03.push_back(std::make_pair(dateStringToMonth(db.res->getString("Date"), 6) + 0.05, db.res->getDouble("TotalPrice")));
			}
		}
	}

	db.QueryResult(strSqlQ4);
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (data04.size() > 0 && data04.back().first == (dateStringToMonth(db.res->getString("Date"), 9) + 0.15)) {
				data04.back().second = data04.back().second + db.res->getDouble("TotalPrice");
			}
			else {
				data04.push_back(std::make_pair(dateStringToMonth(db.res->getString("Date"), 9) + 0.15, db.res->getDouble("TotalPrice")));
			}
		}
	}
	db.~DBConnection();
}

void SaleAnalysis::generateSemiAnnualComparisonBarGraph(int year) {
	LoadSemiAnnualFromDB_2(year);

	data01.push_back(make_pair(6.87, countSale(data01)));
	data02.push_back(make_pair(7.13, countSale(data02)));

	Gnuplot gp(gnuPath);
	 

	string startRange = "0";
	string endRange = "7.5";
	gp << "set term qt size 1600,900 title 'Sale Report Analysis'\n"; 
	gp << "set xrange ['" << startRange << "':'" << endRange << "']\n";
	gp << "set xtics (\"1st Month\" 1, \"2nd Month\" 2, \"3rd Month\" 3, \"4th Month\" 4,\"5th Month\" 5,\"6th Month\" 6,\"Total All 6 Month\" 7) font \", 12\"\n";
	gp << "set title 'Semi annual analysis " + to_string(year) + "' font \"face, 20\"\n";
	gp << "set key font \"face, 14\"\n";
	gp << "set key spacing 1.3\n";
	gp << "set boxwidth 0.25\n"; 
	gp << "set ylabel \"Total Sale\" font \"face, 15\"\n";
	gp << "set style line 100 lt 1 lc rgb \"black\" lw 0.25\n";
	gp << "set grid ls 100\n"; 
	gp << "set key center top\n";
	gp << "set style fill solid 1.0\n"; 
	if (data01.size() > 1) {
		if (data02.size() > 1) {
			gp << "plot '-' using 1:2 with boxes title 'First Half',";
			gp << "'-'using 1:2 with boxes title 'Second Half'\n";
		}
		else {
			gp << "plot '-' using 1:2 with boxes title 'First Half'\n";
		}
		if (data01.size() > 1)
			gp.send1d(data01);
		if (data02.size() > 1)
			gp.send1d(data02);
	}
	else {
		cout << "No data to be analyzed";
		_getch();
		return;
	} 
	_getch();
	return;
}
void SaleAnalysis::LoadSemiAnnualFromDB_2(int year) {
	data01.clear();
	data02.clear();
	string strSql = "SELECT DATE(CompleteDate) AS Date,TotalPrice FROM orders WHERE ShopId=" + to_string(ShopId) + " AND YEAR(CompleteDate)='" + to_string(year) + "' ";
	string strSqlQ1 = strSql + " AND MONTH(CompleteDate)>=1 AND MONTH(CompleteDate)<=6 ORDER BY CompleteDate ASC";
	string strSqlQ2 = strSql + " AND MONTH(CompleteDate)>=7 AND MONTH(CompleteDate)<=12 ORDER BY CompleteDate ASC";

	DBConnection db;
	db.QueryResult(strSqlQ1);
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (data01.size() > 0 && (data01.back().first== (dateStringToMonth(db.res->getString("Date"))-0.13))) {
				data01.back().second = data01.back().second + db.res->getDouble("TotalPrice");
			}
			else {
				data01.push_back(std::make_pair(dateStringToMonth(db.res->getString("Date")) - 0.13, db.res->getDouble("TotalPrice")));
			}
		}
	}

	db.QueryResult(strSqlQ2);
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (data02.size() > 0 && data02.back().first == (dateStringToMonth(db.res->getString("Date"),6) + 0.13)) {
				data02.back().second = data02.back().second + db.res->getDouble("TotalPrice");
			}
			else {
				data02.push_back(std::make_pair(dateStringToMonth(db.res->getString("Date"),6) + 0.13, db.res->getDouble("TotalPrice")));
			}
		}
	}

	db.~DBConnection();
}

void SaleAnalysis::generateAnnualComparisonLineGraph(int year) {
	LoadAnnualFromDBByDay(year);
	Gnuplot gp(gnuPath);


	string startRange = "1";
	string endRange = "366"; 
	double pyearSale = countSale(data02);
	gp << "set term qt size 1600,900 title 'Sale Report Analysis'\n";
	gp << "set xrange ['" << startRange << "':'" << endRange << "']\n";
	gp << "set title 'Annual analysis' font \"face, 20\"\n";
	gp << "set key font \"face, 12\"\n";
	gp << "set key spacing 1.5\n";
	gp << "set ylabel \"Total Sale\" font \"face, 15\"\n";
	gp << "set xlabel \"Day\" font \"face, 15\"\n";
	gp << "set style line 100 lt 1 lc rgb \"black\" lw 0.25\n";
	gp << "set grid ls 100\n";
	if (data01.size() > 1) {
		if (pyearSale > 0) {
			gp << "plot '-' using 1:2 with linespoints  title '" + to_string(year) + "' lw 1.5,";
			gp << "'-'using 1:2 with  linespoints  title '" + to_string(year -1) + "' lw 1.5\n";
		}
		else {
			gp << "plot '-' using 1:2 with linespoints  title '" + to_string(year) + "' lw 1.5\n";
		}
		if (data01.size() > 1)
			gp.send1d(data01);
		if (pyearSale > 0)
			gp.send1d(data02);
	}
	else {
		cout << "No data to be analyzed";
		_getch();
		return;
	}  
	_getch();
	return;
}
void SaleAnalysis::LoadAnnualFromDBByDay(int year) {
	data01.clear();
	data02.clear();
	int maxDay = 365;
	int d = 1;
	string strSqlQ1 = "SELECT DATE(CompleteDate) AS Date,TotalPrice FROM orders WHERE ShopId=" + to_string(ShopId) + " AND YEAR(CompleteDate)='" + to_string(year) + "' ORDER BY CompleteDate ASC";
	string strSqlQ2 = "SELECT DATE(CompleteDate) AS Date,TotalPrice FROM orders WHERE ShopId=" + to_string(ShopId) + " AND YEAR(CompleteDate)='" + to_string(year - 1) + "' ORDER BY CompleteDate ASC";


	if (year % 4 == 0 && year % 100 != 0) {
		maxDay = 366; //leap 365 
	}

	populateZero(&data01, maxDay); 
	DBConnection db;
	db.QueryResult(strSqlQ1);
	if (db.res->rowsCount() > 0) {
		d = 1;
		while (db.res->next()) {
			int cDay = dateStringToDayFromBase(db.res->getString("Date"));
			if (data01[cDay].second != 0) {
				data01[cDay].second += db.res->getDouble("TotalPrice");
			}
			else {
				data01[cDay].second = db.res->getDouble("TotalPrice");
			}
		}
	} 
	maxDay = 365;
	if ((year - 1) % 4 == 0 && (year - 1) % 100 != 0) {
		maxDay = 366; //leap 365 
	}
	populateZero(&data02, maxDay);

	db.QueryResult(strSqlQ2);
	if (db.res->rowsCount() > 0) {
		d = 1;
		while (db.res->next()) {
			int cDay = dateStringToDayFromBase(db.res->getString("Date"));
			if (data02[cDay].second != 0) {
				data02[cDay].second += db.res->getDouble("TotalPrice");
			}
			else {
				data02[cDay].second = db.res->getDouble("TotalPrice");
			}
		}
	}


	db.~DBConnection();
}

void SaleAnalysis::generateQuarterComparisonLineGraph(int year) {
	LoadQuartersFromDBByDay(year);
	Gnuplot gp(gnuPath);

	double q1Sale = countSale(data01);
	double q2Sale = countSale(data02);
	double q3Sale = countSale(data03);
	double q4Sale = countSale(data04);
	string startRange = "1";
	string endRange = "95";
	gp << "set term qt size 1600,900 title 'Sale Report Analysis'\n";
	gp << "set xrange ['" << startRange << "':'" << endRange << "']\n";
	gp << "set title 'Quarterly analysis " + to_string(year) + "' font \"face, 20\"\n";
	gp << "set key font \"face, 10\"\n";
	gp << "set key spacing 1.2\n";
	gp << "set ylabel \"Total Sale\" font \"face, 15\"\n";
	gp << "set xlabel \"Day\" font \"face, 15\"\n";
	gp << "set style line 100 lt 1 lc rgb \"black\" lw 0.25\n";
	gp << "set grid ls 100\n";
	if (q1Sale > 0) {
		if (q2Sale > 0) {
			gp << "plot '-' using 1:2 with linespoints  title 'First Quarter' lw 1.5,";
			if (q3Sale > 0) {
				gp << "'-'using 1:2 with linespoints  title 'Second Quarter' lw 1.5,";
				if (q4Sale > 0) {
					gp << "'-'using 1:2 with linespoints  title 'Third Quarter' lw 1.5,";
					gp << "'-'using 1:2 with linespoints  title 'Fourth Quarter' lw 1.5\n";
				}
				else {
					gp << "'-'using 1:2 with linespoints  title 'Third Quarter' lw 1.5\n";
				}
			}
			else {
				gp << "'-'using 1:2 with lines linespoints  'Second Quarter' lw 1.5\n";
			}
		}
		else {
			gp << "plot '-' using 1:2 with lines linespoints  'First Quarter' lw 1.5\n";
		}
		if (q1Sale > 0)
			gp.send1d(data01);
		if (q2Sale > 0)
			gp.send1d(data02);
		if (q3Sale > 0)
			gp.send1d(data03);
		if (q4Sale > 0)
			gp.send1d(data04);
	}
	else {
		cout << "No data to be analyzed";
		_getch();
		return;
	}
	_getch();
	return;
}
void SaleAnalysis::LoadQuartersFromDBByDay(int year) {
	data01.clear();
	data02.clear();
	data03.clear();
	data04.clear();
	string strSql = "SELECT DATE(CompleteDate) AS Date,TotalPrice FROM orders WHERE ShopId=" + to_string(ShopId) + " AND YEAR(CompleteDate)='" + to_string(year) + "' ";
	string strSqlQ1 = strSql + " AND MONTH(CompleteDate)>=1 AND MONTH(CompleteDate)<=3 ORDER BY CompleteDate ASC";
	string strSqlQ2 = strSql + " AND MONTH(CompleteDate)>=4 AND MONTH(CompleteDate)<=6 ORDER BY CompleteDate ASC";
	string strSqlQ3 = strSql + " AND MONTH(CompleteDate)>=7 AND MONTH(CompleteDate)<=9 ORDER BY CompleteDate ASC";
	string strSqlQ4 = strSql + " AND MONTH(CompleteDate)>=10 AND MONTH(CompleteDate)<=12 ORDER BY CompleteDate ASC";



	DBConnection db;

	int maxDay = 90;
	if (year % 4 == 0 && year % 100 != 0) {
		maxDay = 91; //leap 
	}
	int d;
	populateZero(&data01, maxDay);

	db.QueryResult(strSqlQ1); 
	if (db.res->rowsCount() > 0) {
		d = 1;
		while (db.res->next()) {
			int cDay = dateStringToDayFromBase(db.res->getString("Date"));
			if (data01[cDay].second != 0) {
				data01[cDay].second += db.res->getDouble("TotalPrice");
			}
			else {
				data01[cDay].second = db.res->getDouble("TotalPrice");
			}
		}
	}
	 
	maxDay = 91; 
	populateZero(&data02, maxDay);
	db.QueryResult(strSqlQ2);
	if (db.res->rowsCount() > 0) {
		d = 1;
		while (db.res->next()) {
			int cDay = dateStringToDayFromBase(db.res->getString("Date"), 4);
			if (data02[cDay].second != 0) {
				data02[cDay].second += db.res->getDouble("TotalPrice");
			}
			else {
				data02[cDay].second = db.res->getDouble("TotalPrice");
			}
		}
	}

	maxDay = 92; 
	populateZero(&data03, maxDay);
	db.QueryResult(strSqlQ3);
	if (db.res->rowsCount() > 0) {
		d = 1;
		while (db.res->next()) {
			int cDay = dateStringToDayFromBase(db.res->getString("Date"), 7);
			if (data03[cDay].second != 0) {
				data03[cDay].second += db.res->getDouble("TotalPrice");
			}
			else {
				data03[cDay].second = db.res->getDouble("TotalPrice");
			}
		}
	}


	maxDay = 92;
	populateZero(&data04, maxDay);
	db.QueryResult(strSqlQ4);
	if (db.res->rowsCount() > 0) {
		d = 1;
		while (db.res->next()) {
			int cDay = dateStringToDayFromBase(db.res->getString("Date"), 10);
			if (data04[cDay].second != 0) {
				data04[cDay].second += db.res->getDouble("TotalPrice");
			}
			else {
				data04[cDay].second = db.res->getDouble("TotalPrice");
			}
		}
	}

	db.~DBConnection();
}

void SaleAnalysis::generateSemiAnnualComparisonLineGraph(int year) {
	LoadSemiAnnualFromDBByDay(year);
	Gnuplot gp(gnuPath);

	double secondSale = countSale(data02);
	double firstSale = countSale(data01);
	string startRange = "1";
	string endRange = "185";
	gp << "set term qt size 1600,900 title 'Sale Report Analysis'\n";
	gp << "set xrange ['" << startRange << "':'" << endRange << "']\n"; 
	gp << "set title 'Semi annual analysis " + to_string(year) + "' font \"face, 20\"\n";
	gp << "set key font \"face, 12\"\n";
	gp << "set key spacing 1.5\n";
	gp << "set ylabel \"Total Sale\" font \"face, 15\"\n";
	gp << "set xlabel \"Day\" font \"face, 15\"\n";
	gp << "set style line 100 lt 1 lc rgb \"black\" lw 0.25\n";
	gp << "set grid ls 100\n";
	if (firstSale > 0) {
		if (secondSale > 0) {
			gp << "plot '-' using 1:2 with linespoints  title 'First Half' lw 1.5,";
			gp << "'-'using 1:2 with linespoints title 'Second Half' lw 1.5\n";
		}
		else {
			gp << "plot '-' using 1:2 with linespoints  title 'First Half' lw 1.5\n";
		}
		if (data01.size() > 1)
			gp.send1d(data01);
		if (secondSale > 0)
			gp.send1d(data02);
	}
	else {
		cout << "No data to be analyzed";
		_getch();
		return;
	}
	_getch();
	return;
}
void SaleAnalysis::LoadSemiAnnualFromDBByDay(int year) {
	data01.clear();
	data02.clear();
	string strSql = "SELECT DATE(CompleteDate) AS Date,TotalPrice FROM orders WHERE ShopId=" + to_string(ShopId) + " AND YEAR(CompleteDate)='" + to_string(year) + "' ";
	string strSqlQ1 = strSql + " AND MONTH(CompleteDate)>=1 AND MONTH(CompleteDate)<=6 ORDER BY CompleteDate ASC";
	string strSqlQ2 = strSql + " AND MONTH(CompleteDate)>=7 AND MONTH(CompleteDate)<=12 ORDER BY CompleteDate ASC";



	DBConnection db;

	int maxDay = 181;
	if (year % 4 == 0 && year % 100 != 0) {
		maxDay = 182; //leap 
	}
	int d;
	populateZero(&data01, maxDay);
	db.QueryResult(strSqlQ1);
	if (db.res->rowsCount() > 0) {
		d = 1;
		while (db.res->next()) {
			int cDay = dateStringToDayFromBase(db.res->getString("Date"));
			if (data01[cDay].second != 0) {
				data01[cDay].second += db.res->getDouble("TotalPrice");
			}
			else {
				data01[cDay].second = db.res->getDouble("TotalPrice");
			}
		}
	}


	maxDay = 184;  
	populateZero(&data02, maxDay);
	db.QueryResult(strSqlQ2);
	if (db.res->rowsCount() > 0) {
		d = 1;
		while (db.res->next()) {
			int cDay = dateStringToDayFromBase(db.res->getString("Date"),7);
			if (data02[cDay].second != 0) {
				data02[cDay].second += db.res->getDouble("TotalPrice");
			}
			else {
				data02[cDay].second = db.res->getDouble("TotalPrice");
			}
		}
	}


	db.~DBConnection();  
}



//utils

int SaleAnalysis::dateStringToMonth(string date, int d) {
	int s = date.find("-");
	int e = date.find_last_of("-");
	int mont = stoi(date.substr(s + 1, e));
	return mont - d;
}


int SaleAnalysis::dateStringToDayFromBase(string date, int base) {
	int month = dateStringToMonth(date);
	int year = parseDateToYear(date);
	int day = parseDateToDay(date);
	bool leap = (year % 4 == 0 && year % 100 != 0);
	int dayInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
	if (leap)
		dayInMonth[2] = 29;
	if (month < base)
		return -1;
	if (month == base)
		return day;
	else {
		for (int i = base; i < month; i++) {
			day += dayInMonth[i];
		}
		return day;
	}

}

int SaleAnalysis::parseDateToYear(string date) {
	std::size_t found = date.find("-");
	date = date.substr(0, found);
	return stoi(date);
}
int SaleAnalysis::parseDateToDay(string date) { 
	std::size_t found = date.find_last_of("-");
	date = date.substr(found + 1); 
	return stoi(date);
}
 
string SaleAnalysis::decrementDateMonth(string date, int d) {

	int s = date.find("-");
	int e = date.find_last_of("-");
	int mont = stoi(date.substr(s + 1, e));
	if ((mont - d) < 10)
		return date.substr(0, s + 1) + "0" + to_string(mont - d) + date.substr(e);
	else
		return date.substr(0, s + 1) + to_string(mont - d) + date.substr(e);
}


double SaleAnalysis::countSale(std::vector<std::pair<std::string, double>> toCount) {
	double total = 0;
	for (int i = 0; i < toCount.size(); i++) {
		total += toCount[i].second;
	}
	return total;
}


double SaleAnalysis::countSale(std::vector<std::pair<double, double>> toCount) {
	double total = 0;
	for (int i = 0; i < toCount.size(); i++) {
		total += toCount[i].second;
	}
	return total;
}


void SaleAnalysis::populateZero(std::vector<std::pair<double, double>> * vect,int last) {
	vect->clear();
	for (int i = 0; i <= last; i++) {
		vect->push_back(std::make_pair(i, 0));
	}
}


double SaleAnalysis::RoundDecimal(double  d, int place) {
	d = d * pow(10, place);
	d = round(d);
	return d * pow(10, -place);
}


void SaleAnalysis::LoadProductSoldCount(int year) {
	productSoldCount.clear();
	DBConnection db;
	int curId = -1;
	db.QueryResult("SELECT i.ProductId,i.quantity,p.Name FROM orders o, orderitem i, product p WHERE o.OrderId=i.OrderId AND i.ProductId = p.ProductId AND YEAR(o.CompleteDate)='" + to_string(year) +"' AND o.ShopId="+to_string(ShopId) + " ORDER BY i.ProductId ASC");
	if (db.res->rowsCount() > 0) {
		while (db.res->next()) {
			if (curId == -1) {
				curId = db.res->getInt("ProductId");
				productSoldCount.push_back(std::make_pair(db.res->getString("Name"), db.res->getInt("quantity")));
			}
			else if (curId == db.res->getInt("ProductId")) {
				productSoldCount.back().second += db.res->getInt("quantity");
			}
			else if (curId != db.res->getInt("ProductId")) {
				curId = db.res->getInt("ProductId");
				productSoldCount.push_back(std::make_pair(db.res->getString("Name"), db.res->getInt("quantity")));
			}
		}
	}
	if (productSoldCount.size() > 1) {
		quickSort(&productSoldCount, 0, productSoldCount.size() - 1);
	}
}


void SaleAnalysis::quickSort(vector<pair<string, int>> * arr, int low, int high){
	if (low < high)
	{
		int pi = partition(arr, low, high);
		quickSort(arr, low, pi - 1);  // Before pi
		quickSort(arr, pi + 1, high); // After pi
	}
}

int SaleAnalysis::partition(vector<pair<string, int>> * arr, int low, int high){
	int pivot = arr->at(high).second;
	int i = (low - 1);
	for (int j = low; j <= high - 1; j++) {
		if (arr->at(j).second >= pivot) {
			i++; 
			pair<string, int> tmp;
			tmp.first = arr->at(j).first;
			tmp.second = arr->at(j).second;
			arr->at(j).first = arr->at(i).first;
			arr->at(j).second = arr->at(i).second;
			arr->at(i).first = tmp.first;
			arr->at(i).second = tmp.second;
		}
	}

	pair<string, int> tmp;
	tmp.first = arr->at(high).first;
	tmp.second = arr->at(high).second;
	arr->at(high).first = arr->at(i+1).first;
	arr->at(high).second = arr->at(i +1).second;
	arr->at(i +1).first = tmp.first;
	arr->at(i + 1).second = tmp.second; 
	return (i + 1);
}
 