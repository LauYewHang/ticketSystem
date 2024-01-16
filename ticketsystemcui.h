#ifndef __TICKETSYSTEMCUI_H_INCLUDED__
#define __TICKETSYSTEMCUI_H_INCLUDED__

#include <cstring>
#include <string>
#include <vector>

namespace tscui {
	struct performanceNameData {
		std::string date, month, year, timeStart, timeEnd, location, title;
	};

	std::string modifyOutputText(std::string text);
	std::string modifyStringDouble(double value, int precision = 2);
	std::string modifyStringLowerCase(std::string text);

	void systemLoginMenu();

	void endDisplayDeco();
	void endScene();
	void switchPositionMenu(std::string user, std::string password, std::string position);
	void displayWelcomeUI(std::string user, std::string position);

	void adminMainMenu(std::string user, std::string password, std::string position);

	void managerMainMenu(std::string user, std::string password, std::string position);

	void systemViewEventMenu(std::string user, std::string password, std::string position);
	void systemViewEventDetail(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position);
	void systemModifyEventMenu(std::string user, std::string password, std::string position);
	void systemModifyEventDetail(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position);
	void systemEventModifySeat(std::string eventFileName, performanceNameData performanceDetail, std::string modifyType, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix);
	void systemEventModifyAgePrice(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix);
	void systemEventModifyRowPrice(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix);

	void readPerformanceList(std::vector<std::string>* performanceListPtr, std::string user, std::string password, std::string position);
	void readPerformanceNameData(std::string fileName, std::vector <performanceNameData>* performanceNameDataListPtr);
	void modifyFileData(std::string filePath, std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix);
}

#endif //ticketsystemcui.h
