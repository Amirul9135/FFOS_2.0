#pragma once
#pragma once
#ifndef SaleAnalysis_H
#define SaleAnalysis_H 
#include <vector> 

class SaleAnalysis {
public:
	SaleAnalysis(int); 
	void doQuarterAnalysis(int year);
	void doSemiAnnualAnalysis(int year);
	void doAnnualAnalysis(int year);
	void generateAnnualComparisonBarGraph(int year);
	void generateAnnualComparisonLineGraph(int year);
	void generateQuarterComparisonBarGraph(int year);
	void generateQuarterComparisonLineGraph(int year);
	void generateSemiAnnualComparisonBarGraph(int year);
	void generateSemiAnnualComparisonLineGraph(int year);
private:
	int ShopId; 
	std::string gnuPath;
	std::vector<std::pair<double, double>> data01;
	std::vector<std::pair<double, double>> data02;
	std::vector<std::pair<double, double>> data03;
	std::vector<std::pair<double, double>> data04;
	std::vector<std::pair<std::string, int>> productSoldCount;
	void LoadAnnualFromDBByMonth(int year); // bar
	void LoadSemiAnnualFromDB_2(int year); //bar
	void LoadQuartersFromDB_2(int year); // bar
	void LoadAnnualFromDBByDay(int year);
	void LoadSemiAnnualFromDBByDay(int year);
	void LoadQuartersFromDBByDay(int year);
	std::string decrementDateMonth(std::string, int);
	int dateStringToMonth(std::string, int d = 0);
	int dateStringToDayFromBase(std::string, int base = 1);
	int parseDateToYear(std::string date);
	int parseDateToDay(std::string date);
	double countSale(std::vector<std::pair<std::string, double>>);
	double countSale(std::vector<std::pair<double, double>>);
	void populateZero(std::vector<std::pair<double, double>> *, int lastIndex);
	double RoundDecimal(double, int place);
	void quickSort(std::vector<std::pair<std::string, int>>* arr, int low, int high);
	int partition(std::vector<std::pair<std::string, int>>* arr, int low, int high);
	void LoadProductSoldCount(int year);
	void replaceAll(std::string& str, const std::string& from, const std::string& to);
};
#endif  