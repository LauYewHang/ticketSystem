#include "ticketsystemcui.h"
#include "integervalidate.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>
#include <array>
#include <vector>
#include <chrono>
#include <thread>

const int UISIZE = 106;

std::string tscui::modifyOutputText(std::string text) {
	if ((text.size() % 2) != 0) {
		text = text + " ";
	}
	return text;
}

std::string tscui::modifyStringDouble(double value, int precision) {
	std::string stringDouble = std::to_string(value), realStringDouble = "";
	for (int i = 0; i < stringDouble.size() - (6 - precision); i++) {
		realStringDouble = realStringDouble + stringDouble[i];
	}
	return realStringDouble;
}

std::string tscui::modifyStringLowerCase(std::string text) {
	std::string realString = "";
	for (int i = 0; i < text.size(); i++) {
		if (int(text[i]) >= 65 && int(text[i]) <= 90) {
			realString += char(int(text[i]) + 32);
		}
		else {
			realString += text[i];
		}
	}
	return realString;
}

void tscui::systemLoginMenu() {
	int currentOutputChar = 45, loginAttempt = 3;
	std::string currentOutputText = "", inputUsername = "", inputPassword = "";
	bool loginApprove = false;

	struct systemUser {
		std::string username, password, position;
	};
	std::vector <systemUser> systemUserList;

	std::fstream fileData;
	fileData.open("ticketSystemUsers.txt", std::ios_base::in | std::ios_base::out);
	if (fileData.is_open()) {
		std::string flowUsername = "", flowPassword = "", flowPosition = "";
		while (fileData >> flowUsername >> flowPassword >> flowPosition) {
			if (flowUsername != "" && flowPassword != "" && flowPosition != "") {
				systemUserList.emplace_back();
				systemUserList[systemUserList.size() - 1] = { flowUsername, flowPassword, flowPosition };
				flowUsername = "";
				flowPassword = "";
				flowPosition = "";
			}
		}
	}
	else {
		std::cerr << "Error: Unable to locate crucial file: \"ticketSystemUsers.txt\"." << std::endl << "Terminating program..." << std::endl << "Exit status: 1";
		std::exit(1);
	}

	std::cout << "\x1b[24;40m";
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Ticket Sales System";
	std::cout << char(currentOutputChar) << std::setfill(' ') << std::setw((UISIZE - 2) / 2 - (currentOutputText.size() + 9) / 2) << "" << "\x1b[1;91m" << "UOW SCCA" << "\x1b[97m" << " " << "\x1b[94m" << currentOutputText << "\x1b[22;97m" << std::setw(UISIZE - currentOutputText.size() - ((UISIZE - 2) / 2 - (currentOutputText.size() - 9) / 2) - 1) << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Please log in to proceed";
	std::cout << char(currentOutputChar) << std::setfill(' ') << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << "\x1b[4m" << currentOutputText << "\x1b[24m" << std::setw(UISIZE - currentOutputText.size() - ((UISIZE - 2) / 2 - currentOutputText.size() / 2) - 1) << char(currentOutputChar) << std::endl;
	for (int i = 0; i < 5; i++) {
		std::cout << char(currentOutputChar) << std::setfill(' ') << std::setw(UISIZE - 1) << char(currentOutputChar) << std::endl;
	}
	currentOutputText = " Username: ";
	std::cout << char(currentOutputChar) << currentOutputText << std::setfill(' ') << std::setw(UISIZE - 1 - currentOutputText.size()) << char(currentOutputChar) << std::endl;
	currentOutputText = " Password: ";
	std::cout << char(currentOutputChar) << currentOutputText << std::setfill(' ') << std::setw(UISIZE - 1 - currentOutputText.size()) << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	std::cout << "\x1b[3A\x1b[12C";
	getline(std::cin, inputUsername);
	std::cout << "\x1b[12C";
	getline(std::cin, inputPassword);
	while (!(loginApprove) && loginAttempt > 0) {
		for (int i = 0; i < systemUserList.size(); i++) {
			if (inputUsername == systemUserList[i].username) {
				if (inputPassword == systemUserList[i].password) {
					loginApprove = true;
					currentOutputText = "Login successful. Welcome " + systemUserList[i].username;
					currentOutputText = tscui::modifyOutputText(currentOutputText);
					currentOutputChar = 124;
					std::cout << "\x1b[6A";
					std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[92m" << currentOutputText << "\x1b[97m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
					std::cout << "\x1b[6B";
					std::this_thread::sleep_for(std::chrono::seconds(2));
					std::cout << "\x1b[0m" << "\x1b[2J" << "\x1b[H";
					fileData.close();
					if (systemUserList[i].position == "admin") {
						tscui::adminMainMenu(systemUserList[i].username, systemUserList[i].password, systemUserList[i].position);
					}
					else {
						tscui::managerMainMenu(systemUserList[i].username, systemUserList[i].password, systemUserList[i].position);
					}
				}
			}
		}
		if (!(loginApprove)) {
			loginAttempt -= 1;
			if (loginAttempt > 0) {
				currentOutputText = "Invalid username or password input. Please try again. " + std::to_string(loginAttempt) + " attempt(s) left.";
				currentOutputChar = 124;
				std::cout << "\x1b[6A";
				std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[5;91m" << currentOutputText << "\x1b[25;97m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
				std::cout << "\x1b[3B";
				currentOutputText = " Username: ";
				std::cout << char(currentOutputChar) << currentOutputText << std::setfill(' ') << std::setw(UISIZE - 1 - currentOutputText.size()) << char(currentOutputChar) << std::endl;
				currentOutputText = " Password: ";
				std::cout << char(currentOutputChar) << currentOutputText << std::setfill(' ') << std::setw(UISIZE - 1 - currentOutputText.size()) << char(currentOutputChar) << std::endl;
				std::cout << "\x1b[2A\x1b[12C";
				getline(std::cin, inputUsername);
				std::cout << "\x1b[12C";
				getline(std::cin, inputPassword);
			}
			else {
				for (int i = 3; i >= 0; i--) {
					currentOutputChar = 124;
					std::cout << "\x1b[6A";
					currentOutputText = "Invalid username or password input. You have run out of login attempt.";
					std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
					currentOutputText = "Ending the program in " + std::to_string(i) + " second(s).";
					std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
					std::cout << "\x1b[2B";
					currentOutputText = " Username: ";
					std::cout << char(currentOutputChar) << currentOutputText << std::setfill(' ') << std::setw(UISIZE - 1 - currentOutputText.size()) << char(currentOutputChar) << std::endl;
					currentOutputText = " Password: ";
					std::cout << char(currentOutputChar) << currentOutputText << std::setfill(' ') << std::setw(UISIZE - 1 - currentOutputText.size()) << char(currentOutputChar) << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				std::cout << std::endl << "\x1b[0m";
				endScene();
				fileData.close();
			}
		}
	}
	fileData.close();
	endScene();
	std::cout << std::endl << "\x1b[0m";
}

void tscui::endDisplayDeco() {
	std::cout << "\x1b[0m";
}

void tscui::endScene() {
	std::cout << "\033c";
}

void tscui::switchPositionMenu(std::string user, std::string password, std::string position) {
	if (position == "admin") {
		tscui::adminMainMenu(user, password, position);
	}
	else {
		tscui::managerMainMenu(user, password, position);
	}
}

void tscui::displayWelcomeUI(std::string user, std::string position) {
	int currentOutputChar = 45;
	std::string currentOutputText = "";

	std::cout << "\x1b[24;40m";
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Welcome!";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[91;5m" << currentOutputText << "\x1b[97;25m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
	if (position == "admin") {
		currentOutputText = "Admin: " + user;
	}
	else {
		currentOutputText = "Manager: " + user;
	}
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[96m" << currentOutputText << "\x1b[97m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
}

void tscui::adminMainMenu(std::string user, std::string password, std::string position) {
	int currentOutputChar = 45, commandCode = 0;
	std::string currentOutputText = "", lastOutputText = "", currentOutputDeco = "", userInput = "";
	std::string commandList[] = { "Check event status", "Modify event status"};
	bool commandPass = false;
	void (*callScene)(std::string user, std::string password, std::string position) = [](std::string user, std::string password, std::string position) {};

	std::cout << "\x1b[24;40m";
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Welcome!";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[91;5m" << currentOutputText << "\x1b[97;25m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
	currentOutputText = "Admin: " + user;
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[96m" << currentOutputText << "\x1b[97m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Command code";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
	lastOutputText = currentOutputText;
	currentOutputText = "Function";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	for (int i = 0; i < size(commandList); i++) {
		switch (i) {
		case 0: case 1:
			currentOutputDeco = "\x1b[92m";
			break;
		case 2: case 3:
			currentOutputDeco = "\x1b[33m";
			break;
		case 4: case 5: case 6:
			currentOutputDeco = "\x1b[94m";
			break;
		case 7:
			currentOutputDeco = "\x1b[91m";
			break;
		default:
			currentOutputDeco = "\x1b[97m";
			break;
		}
		currentOutputChar = 124;
		currentOutputText = std::to_string(i+1);
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
		lastOutputText = currentOutputText;
		currentOutputText = commandList[i];
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << std::endl;
	}
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Command code: ";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << std::endl;
	auto invalidCommandCodeUI = []() {
		int currentOutputChar = 0;
		std::string currentOutputText = "";
		std::cout << "\x1b[A";
		currentOutputChar = 124;
		currentOutputText = "Invalid command code, please try again";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		std::cout << "\x1b[A";
		std::this_thread::sleep_for(std::chrono::seconds(2));
		currentOutputText = "Command code: ";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << std::endl;
		std::cout << "\x1b[A\x1b[16C";
	};
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	std::cout << "\x1b[2A\x1b[16C";
	while (!(commandPass)) {
		commandCode = intvalidate::getInteger("", "", false, false, invalidCommandCodeUI);
		if (commandCode >= 1 && commandCode <= size(commandList)) {
			commandPass = true;
			switch (commandCode) {
			case 1:
				currentOutputText = "Entering view status mode";
				callScene = &systemViewEventMenu;
				break;
			case 2:
				currentOutputText = "Entering modify status mode";
				callScene = &systemModifyEventMenu;
				break;
			}
		}
		else {
			invalidCommandCodeUI();
		}
	}
	for (int i = 0; i <= 3; i++) {
		std::cout << "  " << "\x1b[92m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		currentOutputChar = 45;
		std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
		currentOutputText = currentOutputText + ".";
		std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << "\x1b[2A";
	}
	endScene();
	callScene(user, password, position);
}

void tscui::managerMainMenu(std::string user, std::string password, std::string position) {
	int currentOutputChar = 45, commandCode = 0;
	std::string currentOutputText = "", lastOutputText = "", currentOutputDeco = "", userInput = "";
	std::string commandList[] = { "Check event status" };
	bool commandPass = false;
	void (*callScene)(std::string user, std::string password, std::string position) = [](std::string user, std::string password, std::string position) {};

	std::cout << "\x1b[24;40m";
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Welcome!";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[91;5m" << currentOutputText << "\x1b[97;25m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
	currentOutputText = "Admin: " + user;
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << "\x1b[96m" << currentOutputText << "\x1b[97m" << std::setw((UISIZE - 2) / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Command code";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
	lastOutputText = currentOutputText;
	currentOutputText = "Function";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	for (int i = 0; i < size(commandList); i++) {
		switch (i) {
		case 0: case 1:
			currentOutputDeco = "\x1b[92m";
			break;
		case 2: case 3:
			currentOutputDeco = "\x1b[33m";
			break;
		case 4: case 5: case 6:
			currentOutputDeco = "\x1b[94m";
			break;
		case 7:
			currentOutputDeco = "\x1b[91m";
			break;
		default:
			currentOutputDeco = "\x1b[97m";
			break;
		}
		currentOutputChar = 124;
		currentOutputText = std::to_string(i + 1);
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
		lastOutputText = currentOutputText;
		currentOutputText = commandList[i];
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << std::endl;
	}
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Command code: ";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << std::endl;
	auto invalidCommandCodeUI = []() {
		int currentOutputChar = 0;
		std::string currentOutputText = "";
		std::cout << "\x1b[A";
		currentOutputChar = 124;
		currentOutputText = "Invalid command code, please try again";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		std::cout << "\x1b[A";
		std::this_thread::sleep_for(std::chrono::seconds(2));
		currentOutputText = "Command code: ";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << std::endl;
		std::cout << "\x1b[A\x1b[16C";
	};
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	std::cout << "\x1b[2A\x1b[16C";
	while (!(commandPass)) {
		commandCode = intvalidate::getInteger("", "", false, false, invalidCommandCodeUI);
		if (commandCode >= 1 && commandCode <= size(commandList)) {
			commandPass = true;
			switch (commandCode) {
			case 1:
				currentOutputText = "Entering view status mode";
				callScene = &systemViewEventMenu;
				break;
			}
		}
		else {
			invalidCommandCodeUI();
		}
	}
	for (int i = 0; i <= 3; i++) {
		std::cout << "  " << "\x1b[92m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		currentOutputChar = 45;
		std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
		currentOutputText = currentOutputText + ".";
		std::this_thread::sleep_for(std::chrono::seconds(0));
		std::cout << "\x1b[2A";
	}
	endScene();
	callScene(user, password, position);
}

void tscui::systemViewEventMenu(std::string user, std::string password, std::string position) {
	std::vector <std::string> performanceList = {};
	std::vector <performanceNameData> performanceNameDataList = {};
	std::string currentOutputText = "", lastOutputText = "", currentOutputDeco = "";
	bool eventFlag = false;
	char currentOutputChar = 124;
	int eventCode = 0;

	tscui::displayWelcomeUI(user, position);
	tscui::readPerformanceList(&performanceList, user, password, position);
	for (int i = 0; i < performanceList.size(); i++) {
		readPerformanceNameData(performanceList[i], &performanceNameDataList);
	}
	currentOutputText = "Select a performance event to view its status";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Event code";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	currentOutputDeco = "\x1b[94m";
	std::cout << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
	lastOutputText = currentOutputText;
	currentOutputText = "Event";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	currentOutputDeco = "\x1b[96m";
	std::cout << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputDeco = "\x1b[97m";
	for (int i = 0; i < performanceNameDataList.size(); i++) {
		currentOutputChar = 124;
		currentOutputText = std::to_string(i + 1);
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
		lastOutputText = currentOutputText;
		currentOutputText = performanceNameDataList[i].title;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << std::endl;
	}
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Event code: ";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << "  " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	std::cout << "\x1b[2A\x1b[14C";
	auto invalidEventCodeUI = []() {
		int currentOutputChar = 0;
		std::string currentOutputText = "";
		std::cout << "\x1b[A";
		currentOutputChar = 124;
		currentOutputText = "Invalid event code, please try again";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		std::cout << "\x1b[A";
		std::this_thread::sleep_for(std::chrono::seconds(2));
		currentOutputText = "Event code: ";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << std::endl;
		std::cout << "\x1b[A\x1b[16C";
	};
	while (!(eventFlag)) {
		eventCode = intvalidate::getInteger("", "", false, false, invalidEventCodeUI);
		if (eventCode >= 1 && eventCode <= performanceNameDataList.size()) {
			eventFlag = true;
			currentOutputText = "Opening event file: \"" + performanceNameDataList[eventCode - 1].title + "\"";
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			for (int i = 0; i <= 3; i++) {
				std::cout << "  " << "\x1b[92m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
				currentOutputChar = 45;
				std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
				currentOutputText = currentOutputText + ".";
				std::this_thread::sleep_for(std::chrono::milliseconds(0));
				std::cout << "\x1b[2A";
			}
			std::cout << "\x1b[2B";
			tscui::endScene();
			tscui::systemViewEventDetail(performanceList[eventCode - 1], performanceNameDataList[eventCode - 1], user, password, position);
		}
		else {
			invalidEventCodeUI();
		}
	}
	tscui::endDisplayDeco();
}

void tscui::systemViewEventDetail(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position) {
	std::fstream fileData;
	std::string fileName = "performanceDetailFolder\\" + eventFileName + ".txt";

	tscui::displayWelcomeUI(user, position);
	fileData.open(fileName, std::ios_base::in);
	if (fileData.is_open()) {
		void (*callScene)(std::string user, std::string password, std::string position) = [](std::string user, std::string password, std::string position) {};
		int seatRowAmount = 0, seatColumnAmount = 0, availability = 0, commandCode = 0;
		int rowUISize = 12, columnUISize = UISIZE - rowUISize, columnDisplayGap = 3, currentOutputChar = 124;
		double seniorPrice = 0.0, adultPrice = 0.0, childrenPrice = 0.0, totalPaidAmount = 0.0, totalPendingAmount = 0.0;
		bool commandPass = false;
		std::string availabilityString = "0", currentOutputText = "", assistingText = "", lastOutputText = "", currentOutputDeco = "", normalDeco = "\x1b[0m";
		std::string commandList[] = { "Go back to event menu", "Go back to main menu" };
		std::string ageList[] = { "Senior", "Adult", "Children" };
		std::vector <double> rowPrice;
		std::vector <std::vector<int>> seatMatrix;
		std::vector <std::vector<int>> seatAgeMatrix;
		fileData >> seatRowAmount >> seatColumnAmount;
		fileData >> seniorPrice >> adultPrice >> childrenPrice;
		double agePriceList[] = {seniorPrice, adultPrice, childrenPrice};
		for (int i = 0; i < seatRowAmount; i++) {
			double rowPriceFlow = 0.0;
			fileData >> rowPriceFlow;
			rowPrice.push_back(rowPriceFlow);
		}
		for (int i = 0; i < seatRowAmount; i++) {
			seatMatrix.emplace_back();
			for (int j = 0; j < seatColumnAmount; j++) {
				seatMatrix[i].emplace_back();
				fileData >> availabilityString;
				availability = intvalidate::strToInt(availabilityString, false, false);
				seatMatrix[i][j] = availability;
			}
		}
		for (int i = 0; i < seatRowAmount; i++) {
			seatAgeMatrix.emplace_back();
			for (int j = 0; j < seatColumnAmount; j++) {
				seatAgeMatrix[i].emplace_back();
				fileData >> availabilityString;
				availability = intvalidate::strToInt(availabilityString, false, false);
				seatAgeMatrix[i][j] = availability;
			}
		}
		std::cout << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << std::endl;
		currentOutputText = "Currently viewing event: " + performanceDetail.title;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << std::endl;
		std::cout << std::setw(UISIZE) << std::setfill('-') << "" << std::endl;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << std::setw(rowUISize) << std::setfill(' ') << "" << char(currentOutputChar);
		for (int i = 0; i < 30; i++) {
			if (i + 1 <= seatColumnAmount) {
				currentOutputText = std::to_string(i + 1);
				tscui::modifyOutputText(currentOutputText);
				for (int j = 0; j < columnDisplayGap - currentOutputText.size(); j++) {
					std::cout << " ";
				}
				std::cout << currentOutputText;
			}
			else {
				for (int k = 0; k < columnDisplayGap; k++) {
					std::cout << " ";
				}
			}
		}
		std::cout << " " << char(currentOutputChar) << std::endl;
		std::cout << std::setw(UISIZE) << std::setfill('-') << "" << std::endl;
		for (int i = 0; i < seatRowAmount; i++) {
			std::cout << char(currentOutputChar) << std::setw(rowUISize / 2 - 1) << std::setfill(' ') << "" << char(i + 65) << std::setw(rowUISize / 2) << "" << char(currentOutputChar);
			for (int j = 0; j < 30; j++) {
				if (j + 1 <= seatColumnAmount) {
					switch (seatMatrix[i][j]) {
						case 0:
							currentOutputText = "O";
							currentOutputDeco = "\x1b[92m";
							break;
						case 1:
							currentOutputText = "?";
							currentOutputDeco = "\x1b[33m";
							break;
						case 2:
							currentOutputText = "X";
							currentOutputDeco = "\x1b[31m";
							break;
						default:
							currentOutputText = "-";
							currentOutputDeco = "\x1b[90m";
							break;
					}
					tscui::modifyOutputText(currentOutputText);
					for (int k = 0; k < columnDisplayGap - currentOutputText.size(); k++) {
						std::cout << " ";
					}
					std::cout << currentOutputDeco << currentOutputText << "\x1b[97m";
				}
				else {
					for (int k = 0; k < columnDisplayGap; k++) {
						std::cout << " ";
					}
				}
			}
			std::cout << " " << char(currentOutputChar) << std::endl;
		}
		std::cout << std::setw(UISIZE) << std::setfill('-') << "" << std::endl;
		currentOutputText = "Date: " + performanceDetail.date + "/" + performanceDetail.month + "/" + performanceDetail.year;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << std::endl;
		currentOutputText = "Time: " + performanceDetail.timeStart + " - " + performanceDetail.timeEnd;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << std::endl;
		if (performanceDetail.location == "aud") {
			assistingText = "Auditorium";
		}
		else {
			std::string stringFlow = performanceDetail.location;
			assistingText = "Lecture theatre ";
			assistingText += stringFlow[stringFlow.size() - 1];
		}
		currentOutputText = "Location: " + assistingText;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << std::endl;
		std::cout << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << std::endl;
		currentOutputText = "Price list (age)";
		currentOutputDeco = "\x1b[4;40m";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputDeco << currentOutputText << normalDeco << "\x1b[40m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		assistingText = tscui::modifyStringDouble(seniorPrice);
		currentOutputDeco = "\x1b[97;40m";
		currentOutputText = "Senior citizen (age 50 & above): RM" + assistingText;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		assistingText = tscui::modifyStringDouble(adultPrice);
		currentOutputText = "Adult (age 18 & above): RM" + assistingText;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		assistingText = tscui::modifyStringDouble(childrenPrice);
		currentOutputText = "Children (age 17 & below): RM" + assistingText;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputText = "Price list (row)";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << "\x1b[4m" << currentOutputText << normalDeco << "\x1b[40m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		for (int i = 0; i < rowPrice.size(); i++) {
			currentOutputText = "";
			currentOutputText += char(i + 65);
			currentOutputText = currentOutputText + ": RM";
			currentOutputText = currentOutputText + tscui::modifyStringDouble(rowPrice[i]);
			std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		}
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		currentOutputChar = 124;
		currentOutputText = "Payment";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << "\x1b[4m" << currentOutputText << normalDeco << "\x1b[40m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		for (int i = 1; i <= size(ageList); i++) {
			int paid = 0, pending = 0;
			double paidAmount = 0.0, pendingAmount = 0.0;
			for (int j = 0; j < seatRowAmount; j++) {
				for (int k = 0; k < seatColumnAmount; k++) {
					if (seatAgeMatrix[j][k] == i) {
						if (seatMatrix[j][k] == 2) {
							paid++;
							paidAmount = paidAmount + (rowPrice[j] + agePriceList[i-1]);
						}
						else if (seatMatrix[j][k] == 1) {
							pending++;
							pendingAmount = pendingAmount + (rowPrice[j] + agePriceList[i-1]);
						}
					}
				}
			}
			currentOutputText = "";
			currentOutputText += char(i + 65 - 1);
			currentOutputText = currentOutputText + ") ";
			currentOutputText = currentOutputText + ageList[i - 1];
			std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
			currentOutputText = "   ";
			currentOutputText = currentOutputText + "Booked: " + std::to_string(pending) + "; " "Payment pending: \x1b[91mRM" + tscui::modifyStringDouble(pendingAmount) + "\x1b[97m";
			currentOutputText = modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3 + 10) << "" << char(currentOutputChar) << normalDeco << std::endl;
			currentOutputText = "   ";
			currentOutputText = currentOutputText + "Paid:   " + std::to_string(paid) + "; Payment received: \x1b[92mRM" + tscui::modifyStringDouble(paidAmount) + "\x1b[97m";
			currentOutputText = modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3 + 10) << "" << char(currentOutputChar) << normalDeco << std::endl;
			totalPaidAmount = totalPaidAmount + paidAmount;
			totalPendingAmount = totalPendingAmount + pendingAmount;
		}
		std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputText = "Total amount pending: \x1b[91mRM" + tscui::modifyStringDouble(totalPendingAmount) + "\x1b[97m";
		currentOutputText = modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3 + 10) << "" << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputText = "Total amount paid   : \x1b[92mRM" + tscui::modifyStringDouble(totalPaidAmount) + "\x1b[97m";
		currentOutputText = modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3 + 10) << "" << char(currentOutputChar) << normalDeco << std::endl;
		std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		currentOutputChar = 124;
		currentOutputText = "Command code";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
		lastOutputText = currentOutputText;
		currentOutputText = "Function";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		for (int i = 0; i < size(commandList); i++) {
			currentOutputChar = 124;
			currentOutputText = std::to_string(i + 1);
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
			lastOutputText = currentOutputText;
			currentOutputText = commandList[i];
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << normalDeco << std::endl;
		}
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		currentOutputChar = 124;
		currentOutputText = "Command code: ";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << normalDeco << std::endl;
		auto invalidCommandCodeUI = []() {
			int currentOutputChar = 0;
			std::string currentOutputText = "", currentOutputDeco = "\x1b[40;97m", normalDeco = "\x1b[0m";
			std::cout << "\x1b[A";
			currentOutputChar = 124;
			currentOutputText = "Invalid command code, please try again";
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << normalDeco << std::endl;
			std::cout << "\x1b[A";
			std::this_thread::sleep_for(std::chrono::seconds(2));
			currentOutputText = "Command code: ";
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << normalDeco << std::endl;
			std::cout << "\x1b[A\x1b[16C" << currentOutputDeco;
		};
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		std::cout << "\x1b[2A\x1b[16C" << currentOutputDeco;
		while (!(commandPass)) {
			commandCode = intvalidate::getInteger("", "", false, false, invalidCommandCodeUI);
			if (commandCode >= 1 && commandCode <= size(commandList)) {
				commandPass = true;
				switch (commandCode) {
				case 1:
					currentOutputText = "Going back to event menu";
					callScene = &systemViewEventMenu;
					break;
				case 2:
					currentOutputText = "Going back to main menu";
					callScene = &switchPositionMenu;
					break;
				}
			}
			else {
				invalidCommandCodeUI();
			}
		}
		for (int i = 0; i <= 3; i++) {
			std::cout << "  " << "\x1b[92m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
			currentOutputChar = 45;
			std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << "\x1b[0m" << std::endl;
			currentOutputText = currentOutputText + ".";
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "\x1b[2A\x1b[40m";
		}
		endScene();
		callScene(user, password, position);
	}
	else {
		endScene();
		int currentOutputChar = 45;
		std::string currentOutputText = "";
		std::cout << "\x1b[40m" << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
		currentOutputText = "Unable to locate file: \"performanceDetailFolder\\" + eventFileName + ".txt";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		currentOutputText = "Returning to event menu screen...";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << "\x1b[0m" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		endScene();
		systemViewEventMenu(user, password, position);
	}
}

void tscui::systemModifyEventMenu(std::string user, std::string password, std::string position) {
	std::vector <std::string> performanceList = {};
	std::vector <performanceNameData> performanceNameDataList = {};
	std::string currentOutputText = "", lastOutputText = "", currentOutputDeco = "";
	bool eventFlag = false;
	char currentOutputChar = 124;
	int eventCode = 0;

	tscui::displayWelcomeUI(user, position);
	tscui::readPerformanceList(&performanceList, user, password, position);
	for (int i = 0; i < performanceList.size(); i++) {
		readPerformanceNameData(performanceList[i], &performanceNameDataList);
	}
	currentOutputText = "Select a performance event to view its status";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Event code";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	currentOutputDeco = "\x1b[94m";
	std::cout << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
	lastOutputText = currentOutputText;
	currentOutputText = "Event";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	currentOutputDeco = "\x1b[96m";
	std::cout << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputDeco = "\x1b[97m";
	for (int i = 0; i < performanceNameDataList.size(); i++) {
		currentOutputChar = 124;
		currentOutputText = std::to_string(i + 1);
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
		lastOutputText = currentOutputText;
		currentOutputText = performanceNameDataList[i].title;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << std::endl;
	}
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	currentOutputChar = 124;
	currentOutputText = "Event code: ";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << "  " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
	currentOutputChar = 45;
	std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
	std::cout << "\x1b[2A\x1b[14C";
	auto invalidEventCodeUI = []() {
		int currentOutputChar = 0;
		std::string currentOutputText = "";
		std::cout << "\x1b[A";
		currentOutputChar = 124;
		currentOutputText = "Invalid event code, please try again";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		std::cout << "\x1b[A";
		std::this_thread::sleep_for(std::chrono::seconds(2));
		currentOutputText = "Event code: ";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << std::endl;
		std::cout << "\x1b[A\x1b[16C";
	};
	while (!(eventFlag)) {
		eventCode = intvalidate::getInteger("", "", false, false, invalidEventCodeUI);
		if (eventCode >= 1 && eventCode <= performanceNameDataList.size()) {
			eventFlag = true;
			currentOutputText = "Opening event file: \"" + performanceNameDataList[eventCode - 1].title + "\"";
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			for (int i = 0; i <= 3; i++) {
				std::cout << "  " << "\x1b[92m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
				currentOutputChar = 45;
				std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
				currentOutputText = currentOutputText + ".";
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				std::cout << "\x1b[2A";
			}
			std::cout << "\x1b[2B";
			tscui::endScene();
			tscui::systemModifyEventDetail(performanceList[eventCode - 1], performanceNameDataList[eventCode - 1], user, password, position);
		}
		else {
			invalidEventCodeUI();
		}
	}
	tscui::endDisplayDeco();
}

void tscui::systemModifyEventDetail(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position) {
	std::fstream fileData;
	std::string fileName = "performanceDetailFolder\\" + eventFileName + ".txt";

	tscui::displayWelcomeUI(user, position);
	fileData.open(fileName, std::ios_base::in);
	if (fileData.is_open()) {
		void (*callScene)(std::string user, std::string password, std::string position) = [](std::string user, std::string password, std::string position) {};
		void (*callModifyScene)(std::string eventFileName, performanceNameData performanceDetail, std::string modifyType, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) = [](std::string eventFileName, performanceNameData performanceDetail, std::string modifyType, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) {};
		void (*callModifyAgePrice)(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) = [](std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) {};
		void (*callModifyRowPrice)(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) = [](std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) {};
		int seatRowAmount = 0, seatColumnAmount = 0, availability = 0, commandCode = 0, sleepMilliSecond = 0;
		int rowUISize = 12, columnUISize = UISIZE - rowUISize, columnDisplayGap = 3, currentOutputChar = 124;
		double seniorPrice = 0.0, adultPrice = 0.0, childrenPrice = 0.0, totalPaidAmount = 0.0, totalPendingAmount = 0.0;
		bool commandPass = false;
		std::string availabilityString = "0", currentOutputText = "", assistingText = "", lastOutputText = "", currentOutputDeco = "", normalDeco = "\x1b[0m";
		std::string modifyAction = "";
		std::string commandList[] = { "Go back to event menu", "Go back to main menu", "Book seat", "Comfirm seat", "Remove seat", "Change row price", "Change age price"};
		std::string ageList[] = { "Senior", "Adult", "Children" };
		std::vector <double> rowPrice;
		std::vector <std::vector<int>> seatMatrix;
		std::vector <std::vector<int>> seatAgeMatrix;
		fileData >> seatRowAmount >> seatColumnAmount;
		fileData >> seniorPrice >> adultPrice >> childrenPrice;
		double agePriceList[] = { seniorPrice, adultPrice, childrenPrice };
		for (int i = 0; i < seatRowAmount; i++) {
			double rowPriceFlow = 0.0;
			fileData >> rowPriceFlow;
			rowPrice.push_back(rowPriceFlow);
		}
		for (int i = 0; i < seatRowAmount; i++) {
			seatMatrix.emplace_back();
			for (int j = 0; j < seatColumnAmount; j++) {
				seatMatrix[i].emplace_back();
				fileData >> availabilityString;
				availability = intvalidate::strToInt(availabilityString, false, false);
				seatMatrix[i][j] = availability;
			}
		}
		for (int i = 0; i < seatRowAmount; i++) {
			seatAgeMatrix.emplace_back();
			for (int j = 0; j < seatColumnAmount; j++) {
				seatAgeMatrix[i].emplace_back();
				fileData >> availabilityString;
				availability = intvalidate::strToInt(availabilityString, false, false);
				seatAgeMatrix[i][j] = availability;
			}
		}
		fileData.close();
		std::cout << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << std::endl;
		currentOutputText = "Currently modifying event: " + performanceDetail.title;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << std::endl;
		std::cout << std::setw(UISIZE) << std::setfill('-') << "" << std::endl;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << std::setw(rowUISize) << std::setfill(' ') << "" << char(currentOutputChar);
		for (int i = 0; i < 30; i++) {
			if (i + 1 <= seatColumnAmount) {
				currentOutputText = std::to_string(i + 1);
				tscui::modifyOutputText(currentOutputText);
				for (int j = 0; j < columnDisplayGap - currentOutputText.size(); j++) {
					std::cout << " ";
				}
				std::cout << currentOutputText;
			}
			else {
				for (int k = 0; k < columnDisplayGap; k++) {
					std::cout << " ";
				}
			}
		}
		std::cout << " " << char(currentOutputChar) << std::endl;
		std::cout << std::setw(UISIZE) << std::setfill('-') << "" << std::endl;
		for (int i = 0; i < seatRowAmount; i++) {
			std::cout << char(currentOutputChar) << std::setw(rowUISize / 2 - 1) << std::setfill(' ') << "" << char(i + 65) << std::setw(rowUISize / 2) << "" << char(currentOutputChar);
			for (int j = 0; j < 30; j++) {
				if (j + 1 <= seatColumnAmount) {
					switch (seatMatrix[i][j]) {
					case 0:
						currentOutputText = "O";
						currentOutputDeco = "\x1b[92m";
						break;
					case 1:
						currentOutputText = "?";
						currentOutputDeco = "\x1b[33m";
						break;
					case 2:
						currentOutputText = "X";
						currentOutputDeco = "\x1b[31m";
						break;
					default:
						currentOutputText = "-";
						currentOutputDeco = "\x1b[90m";
						break;
					}
					tscui::modifyOutputText(currentOutputText);
					for (int k = 0; k < columnDisplayGap - currentOutputText.size(); k++) {
						std::cout << " ";
					}
					std::cout << currentOutputDeco << currentOutputText << "\x1b[97m";
				}
				else {
					for (int k = 0; k < columnDisplayGap; k++) {
						std::cout << " ";
					}
				}
			}
			std::cout << " " << char(currentOutputChar) << std::endl;
		}
		std::cout << std::setw(UISIZE) << std::setfill('-') << "" << std::endl;
		currentOutputText = "Date: " + performanceDetail.date + "/" + performanceDetail.month + "/" + performanceDetail.year;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << std::endl;
		currentOutputText = "Time: " + performanceDetail.timeStart + " - " + performanceDetail.timeEnd;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << std::endl;
		if (performanceDetail.location == "aud") {
			assistingText = "Auditorium";
		}
		else {
			std::string stringFlow = performanceDetail.location;
			assistingText = "Lecture theatre ";
			assistingText += stringFlow[stringFlow.size() - 1];
		}
		currentOutputText = "Location: " + assistingText;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << std::endl;
		std::cout << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << std::endl;
		currentOutputText = "Price list (age)";
		currentOutputDeco = "\x1b[4;40m";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << char(currentOutputChar) << " " << currentOutputDeco << currentOutputText << normalDeco << "\x1b[40m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		assistingText = tscui::modifyStringDouble(seniorPrice);
		currentOutputDeco = "\x1b[97;40m";
		currentOutputText = "Senior citizen (age 50 & above): RM" + assistingText;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		assistingText = tscui::modifyStringDouble(adultPrice);
		currentOutputText = "Adult (age 18 & above): RM" + assistingText;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		assistingText = tscui::modifyStringDouble(childrenPrice);
		currentOutputText = "Children (age 17 & below): RM" + assistingText;
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputText = "Price list (row)";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << "\x1b[4m" << currentOutputText << normalDeco << "\x1b[40m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		for (int i = 0; i < rowPrice.size(); i++) {
			currentOutputText = "";
			currentOutputText += char(i + 65);
			currentOutputText = currentOutputText + ": RM";
			currentOutputText = currentOutputText + tscui::modifyStringDouble(rowPrice[i]);
			std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		}
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		currentOutputChar = 124;
		currentOutputText = "Payment";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << "\x1b[4m" << currentOutputText << normalDeco << "\x1b[40m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
		for (int i = 1; i <= size(ageList); i++) {
			int paid = 0, pending = 0;
			double paidAmount = 0.0, pendingAmount = 0.0;
			for (int j = 0; j < seatRowAmount; j++) {
				for (int k = 0; k < seatColumnAmount; k++) {
					if (seatAgeMatrix[j][k] == i) {
						if (seatMatrix[j][k] == 2) {
							paid++;
							paidAmount = paidAmount + (rowPrice[j] + agePriceList[i - 1]);
						}
						else if (seatMatrix[j][k] == 1) {
							pending++;
							pendingAmount = pendingAmount + (rowPrice[j] + agePriceList[i - 1]);
						}
					}
				}
			}
			currentOutputText = "";
			currentOutputText += char(i + 65 - 1);
			currentOutputText = currentOutputText + ") ";
			currentOutputText = currentOutputText + ageList[i - 1];
			std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3) << "" << char(currentOutputChar) << normalDeco << std::endl;
			currentOutputText = "   ";
			currentOutputText = currentOutputText + "Booked: " + std::to_string(pending) + "; " "Payment pending: \x1b[91mRM" + tscui::modifyStringDouble(pendingAmount) + "\x1b[97m";
			currentOutputText = modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3 + 10) << "" << char(currentOutputChar) << normalDeco << std::endl;
			currentOutputText = "   ";
			currentOutputText = currentOutputText + "Paid:   " + std::to_string(paid) + "; Payment received: \x1b[92mRM" + tscui::modifyStringDouble(paidAmount) + "\x1b[97m";
			currentOutputText = modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3 + 10) << "" << char(currentOutputChar) << normalDeco << std::endl;
			std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(UISIZE-2) << std::setfill(' ') << "" << char(currentOutputChar) << normalDeco << std::endl;
			totalPaidAmount = totalPaidAmount + paidAmount;
			totalPendingAmount = totalPendingAmount + pendingAmount;
		}
		std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputText = "Total amount pending: \x1b[91mRM" + tscui::modifyStringDouble(totalPendingAmount) + "\x1b[97m";
		currentOutputText = modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3 + 10) << "" << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputText = "Total amount paid   : \x1b[92mRM" + tscui::modifyStringDouble(totalPaidAmount) + "\x1b[97m";
		currentOutputText = modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << " " << currentOutputText << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 3 + 10) << "" << char(currentOutputChar) << normalDeco << std::endl;
		std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(UISIZE - 2) << std::setfill(' ') << "" << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		currentOutputChar = 124;
		currentOutputText = "Command code";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
		lastOutputText = currentOutputText;
		currentOutputText = "Function";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << normalDeco << std::endl;
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		for (int i = 0; i < size(commandList); i++) {
			currentOutputChar = 124;
			currentOutputText = std::to_string(i + 1);
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << char(currentOutputChar) << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(18 / 2 - currentOutputText.size() / 2) << "" << char(currentOutputChar);
			lastOutputText = currentOutputText;
			currentOutputText = commandList[i];
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << std::setw((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3) << "" << currentOutputDeco << currentOutputText << "\x1b[97m" << std::setw(UISIZE - (lastOutputText.size() + 2 * (18 / 2 - lastOutputText.size() / 2) + 2 + currentOutputText.size() + ((UISIZE - 2 * (18 / 2 - lastOutputText.size() / 2) - 2) / 3))) << char(currentOutputChar) << normalDeco << std::endl;
		}
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		currentOutputChar = 124;
		currentOutputText = "Command code: ";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << normalDeco << std::endl;
		auto invalidCommandCodeUI = []() {
			int currentOutputChar = 0;
			std::string currentOutputText = "", currentOutputDeco = "\x1b[40;97m", normalDeco = "\x1b[0m";
			std::cout << "\x1b[A";
			currentOutputChar = 124;
			currentOutputText = "Invalid command code, please try again";
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << normalDeco << std::endl;
			std::cout << "\x1b[A";
			std::this_thread::sleep_for(std::chrono::seconds(2));
			currentOutputText = "Command code: ";
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << currentOutputDeco << std::setw(18 / 2 - currentOutputText.size() / 2) << std::setfill(' ') << "" << currentOutputText << std::setw(UISIZE - 2 * (18 / 2 - currentOutputText.size() / 2) - currentOutputText.size() + 2) << "" << normalDeco << std::endl;
			std::cout << "\x1b[A\x1b[16C" << currentOutputDeco;
		};
		currentOutputChar = 45;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		std::cout << "\x1b[2A\x1b[16C" << currentOutputDeco;
		while (!(commandPass)) {
			commandCode = intvalidate::getInteger("", "", false, false, invalidCommandCodeUI);
			if (commandCode >= 1 && commandCode <= size(commandList)) {
				commandPass = true;
				switch (commandCode) {
				case 1:
					currentOutputText = "Going back to event menu";
					callScene = &systemModifyEventMenu;
					sleepMilliSecond = 1000;
					break;
				case 2:
					currentOutputText = "Going back to main menu";
					callScene = &switchPositionMenu;
					sleepMilliSecond = 1000;
					break;
				case 3:
					currentOutputText = "Entering modifying mode (book seat)";
					callModifyScene = &systemEventModifySeat;
					sleepMilliSecond = 500;
					break;
				case 4:
					currentOutputText = "Entering modifying mode (confirm seat)";
					callModifyScene = &systemEventModifySeat;
					sleepMilliSecond = 500;
					break;
				case 5:
					currentOutputText = "Entering modifying mode (remove seat)";
					callModifyScene = &systemEventModifySeat;
					sleepMilliSecond = 500;
					break;
				case 6:
					currentOutputText = "Entering modifying mode (row price list)";
					callModifyRowPrice = &systemEventModifyRowPrice;
					sleepMilliSecond = 500;
					break;
				case 7:
					currentOutputText = "Entering modifying mode (age price list)";
					callModifyAgePrice = &systemEventModifyAgePrice;
					sleepMilliSecond = 500;
					break;
				}	
			}
			else {
				invalidCommandCodeUI();
			}
		}
		for (int i = 0; i <= 3; i++) {
			std::cout << "\x1b[92;40m" << "  " << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << "\x1b[0m" << std::endl;
			currentOutputChar = 45;
			std::cout << "\x1b[97;40m" << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << "\x1b[0m" << std::endl;
			currentOutputText = currentOutputText + ".";
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepMilliSecond));
			std::cout << "\x1b[2A";
		}
		if (commandCode >= 1 && commandCode <= 2) {
			endScene();
			callScene (user, password, position);
		}
		else if (commandCode == 3) {
			std::cout << std::endl << std::endl;;
			callModifyScene (eventFileName, performanceDetail, "book", user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
		}
		else if (commandCode == 4) {
			std::cout << std::endl << std::endl;;
			callModifyScene(eventFileName, performanceDetail, "confirm", user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
		}
		else if (commandCode == 5) {
			std::cout << std::endl << std::endl;;
			callModifyScene(eventFileName, performanceDetail, "remove", user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
		}
		else if (commandCode == 6) {
			std::cout << std::endl << std::endl;;
			callModifyRowPrice(eventFileName, performanceDetail, user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
		}
		else if (commandCode == 7) {
			std::cout << std::endl << std::endl;;
			callModifyAgePrice(eventFileName, performanceDetail, user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
		}
	}
	else {
		endScene();
		int currentOutputChar = 45;
		std::string currentOutputText = "";
		std::cout << "\x1b[40m" << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
		currentOutputText = "Unable to locate file: \"performanceDetailFolder\\" + eventFileName + ".txt";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		currentOutputText = "Returning to event menu screen...";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << "\x1b[0m" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		endScene();
		systemViewEventMenu(user, password, position);
	}
}

void tscui::readPerformanceList(std::vector<std::string>* performanceListPtr, std::string user, std::string password, std::string position) {
	std::fstream fileData;
	fileData.open("performanceDetailFolder\\performanceList.txt", std::ios_base::in);
	if (fileData.is_open()) {
		std::vector <std::string> performanceList;
		std::string performanceFile = "";
		while (fileData >> performanceFile) {
			performanceList.emplace_back();
			performanceList[performanceList.size() - 1] = performanceFile;
		}
		*performanceListPtr = performanceList;
		fileData.close();
	}
	else {
		endScene();
		int currentOutputChar = 45;
		std::string currentOutputText = "";
		std::cout << "\x1b[40m" << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << std::endl;
		currentOutputText = "Unable to locate crucial file: \"performanceDetailFolder\\performanceLista.txt\"";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		currentOutputText = "Returning to menu screen...";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << "  " << "\x1b[91m" << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << std::endl;
		std::cout << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << "\x1b[0m" << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(5));
		endScene();
		switchPositionMenu(user, password, position);
	}
}

void tscui::readPerformanceNameData(std::string fileName, std::vector <performanceNameData>* performanceNameDataListPtr) {
	std::string performanceNameDataArr[] = { "", "", "", "", "", "", ""};
	performanceNameData performanceNameDataFlow;
	for (int i = 0, j = 0; i < fileName.size(); i++) {
		if (fileName[i] != '.') {
			if (fileName[i] == '_') {
				performanceNameDataArr[j] = performanceNameDataArr[j] + " ";
			}
			else {
				performanceNameDataArr[j] = performanceNameDataArr[j] + fileName[i];
			}
		}
		else {
			j++;
		}
	}
	performanceNameDataFlow = {
		performanceNameDataArr[0],
		performanceNameDataArr[1],
		performanceNameDataArr[2],
		performanceNameDataArr[3],
		performanceNameDataArr[4],
		performanceNameDataArr[5],
		performanceNameDataArr[6]
	};
	(*performanceNameDataListPtr).emplace_back();
	(*performanceNameDataListPtr)[performanceNameDataListPtr->size() - 1] = performanceNameDataFlow;
}

void tscui::systemEventModifySeat(std::string eventFileName, performanceNameData performanceDetail, std::string modifyType, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) {
	bool modifyPass = false, agePass = false, seatPass = false, singleSeat = false, canConvertSeat = true;
	int currentOutputChar = 124;
	int ageIndex = 0, startRow = 0, startColumn = 0, endRow = 0, endColumn = 0, singleRow = 0, singleColumn = 0, successfulAdd = 0, failureAdd = 0, splitIndex = 0, modifyMethod = 0;
	std::string rowFlow = "", columnFlow = "", columnFlowStart = "", columnFlowEnd = "";
	std::string filePath = "performanceDetailFolder\\" + eventFileName + ".txt";
	char multipleModifyMethod = ' ';
	std::string currentOutputText = "", cursorUIRewindSpace = "", currentOutputDeco = "\x1b[40;97m", normalDeco = "\x1b[0m";
	std::string inputCommand = "";

	std::cout << "\x1b[3A\x1b[J";
	if (modifyType == "book") {
		currentOutputText = "Book seat";
		modifyMethod = 1;
	}
	else if (modifyType == "confirm") {
		currentOutputText = "Confirm paid seat";
		modifyMethod = 2;
	}
	else if (modifyType == "remove") {
		currentOutputText = "Remove seat";
		modifyMethod = 0;
	}
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	cursorUIRewindSpace = std::to_string(currentOutputText.size() + 2);
	std::cout << currentOutputDeco << "  " << "\x1b[4m" << currentOutputText << "\x1b[24m" << std::setw(UISIZE - currentOutputText.size() - 2) << std::setfill(' ') << "" << normalDeco << std::endl;
	currentOutputChar = 45;
	while ((!(agePass)) && modifyMethod != 0) {
		currentOutputText = "  Age group (Senior / Adult / Children): ";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		cursorUIRewindSpace = std::to_string(currentOutputText.size() - 1);
		std::cout << currentOutputDeco << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << "" << normalDeco << std::endl;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		std::cout << "\x1b[2A\x1b[" + cursorUIRewindSpace + "C" << currentOutputDeco;
		getline(std::cin, inputCommand);
		inputCommand = tscui::modifyStringLowerCase(inputCommand);
		if (inputCommand == "senior") {
			ageIndex = 1;
			agePass = true;
		}
		else if (inputCommand == "adult") {
			ageIndex = 2;
			agePass = true;
		}
		else if (inputCommand == "children") {
			ageIndex = 3;
			agePass = true;
		}
		else {
			currentOutputText = "  Invalid age group entered, please try again";
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			std::cout << "\x1b[A";
		}
	}
	do {
		currentOutputDeco = "\x1b[90;40m";
		std::cout << currentOutputDeco << std::setw(UISIZE) << std::setfill(' ') << "" << normalDeco << std::endl;
		currentOutputText = "  Input format: [Row alphabet][Column number] to modify single seat";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
		currentOutputText = "  [Row alphabet][Column number]-[Row alphabet][Column number] to modify multiple seats in a row";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << "" << normalDeco << std::endl;
		currentOutputText = "  [Row alphabet][Column number]|[Row alphabet][Column number] to modify multiple seats in a column";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << "" << normalDeco << std::endl;
		currentOutputText = "  *You do not need to type the bracket \"[]\"";
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		std::cout << currentOutputDeco << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << "" << normalDeco << std::endl;
		currentOutputDeco = "\x1b[97;40m";
		if (modifyMethod == 1) {
			currentOutputText = "  Enter seat(s) to be booked: ";
		}
		else if (modifyMethod == 2) {
			currentOutputText = "  Enter seat(s) to be confirmed: ";
		}
		else if (modifyMethod == 0) {
			currentOutputText = "  Enter seat(s) to be removed: ";
		}
		currentOutputText = tscui::modifyOutputText(currentOutputText);
		cursorUIRewindSpace = std::to_string(currentOutputText.size());
		std::cout << currentOutputDeco << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << "" << normalDeco << std::endl;
		std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
		std::cout << "\x1b[2A\x1b[" + cursorUIRewindSpace + "C" << currentOutputDeco;
		getline(std::cin, inputCommand);
		splitIndex = 0;
		rowFlow = "";
		columnFlow = "";
		columnFlowStart = "";
		columnFlowEnd = "";
		startRow = 0;
		endRow = 0;
		startColumn = 0;
		endColumn = 0;
		singleSeat = false;
		canConvertSeat = true;
		if (inputCommand.size() <= 3) {
			if (std::tolower(inputCommand[0]) >= 'a' && std::tolower(inputCommand[0]) <= 'z') {
				singleRow = int(std::tolower(inputCommand[0])) - 97;
				for (int i = 1; i < inputCommand.size(); i++) {
					if (inputCommand[i] >= '0' && inputCommand[i] <= '9') {
						columnFlow += inputCommand[i];
					}
					else {
						currentOutputText = "  Invalid input format entered, please try again";
						currentOutputText = tscui::modifyOutputText(currentOutputText);
						std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						std::cout << "\x1b[6A";
						break;
					}
					singleColumn = intvalidate::strToInt(columnFlow, false);
					singleSeat = true;
				}
			}
			else {
				currentOutputText = "  Invalid input format entered, please try again";
				currentOutputText = tscui::modifyOutputText(currentOutputText);
				std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				std::cout << "\x1b[6A";
			}
		}
		else {
			startRow = int(std::tolower(inputCommand[0])) - 97;
			splitIndex = 1;
			for (int i = 1; i < inputCommand.size(); i++) {
				if (splitIndex == 1) {
					if (inputCommand[i] >= '0' && inputCommand[i] <= '9') {
						columnFlowStart += inputCommand[i];
					}
					else if (inputCommand[i] == '-' || inputCommand[i] == '|') {
						multipleModifyMethod = inputCommand[i];
						splitIndex++;
					}
					else {
						canConvertSeat = false;
						break;
					}
				}
				else if (splitIndex == 2) {
					endRow = int(std::tolower(inputCommand[i])) - 97;
					splitIndex++;
				}
				else if (splitIndex == 3) {
					if (inputCommand[i] >= '0' && inputCommand[i] <= '9') {
						columnFlowEnd += inputCommand[i];
					}
					else {
						canConvertSeat = false;
						break;
					}
				}
			}
			if (canConvertSeat && splitIndex == 3) {
				startColumn = intvalidate::strToInt(columnFlowStart, false);
				endColumn = intvalidate::strToInt(columnFlowEnd, false);
			}
			else {
				currentOutputText = "  Invalid input format entered, please try again";
				currentOutputText = tscui::modifyOutputText(currentOutputText);
				std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				std::cout << "\x1b[6A";
			}
		}
		if (singleSeat) {
			singleColumn--;
			if ((singleRow >= 0 && singleRow <= seatRowAmount) && (singleColumn >= 0 && singleColumn <= seatColumnAmount)) {
				if (modifyMethod == 0 && seatAgeMatrix[singleRow][singleColumn] != 0) {
					seatMatrix[singleRow][singleColumn] = modifyMethod;
					seatAgeMatrix[singleRow][singleColumn] = modifyMethod;
					successfulAdd++;
				}
				else if (seatAgeMatrix[singleRow][singleColumn] == 0) {
					seatMatrix[singleRow][singleColumn] = modifyMethod;
					seatAgeMatrix[singleRow][singleColumn] = ageIndex;
					successfulAdd++;
				}
				else if (seatAgeMatrix[singleRow][singleColumn] == ageIndex && modifyMethod == 2) {
					seatMatrix[singleRow][singleColumn] = modifyMethod;
					successfulAdd++;
				}
				else {
					failureAdd++;
				}
				tscui::modifyFileData(filePath, eventFileName, performanceDetail, user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
				seatPass = true;
			}
			else {
				seatPass = false;
				currentOutputText = "  Invalid input format entered, please try again";
				currentOutputText = tscui::modifyOutputText(currentOutputText);
				std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				std::cout << "\x1b[6A";
			}
		}
		else if (canConvertSeat) {
			startColumn--;
			endColumn--;
			if ((startRow >= 0 && endRow <= seatRowAmount) && (startColumn >= 0 && endColumn <= seatColumnAmount)) {
				if (multipleModifyMethod == '-') {
					if (startRow == endRow) {
						for (int i = startColumn; i < endColumn; i++) {
							if (modifyMethod == 0 && seatAgeMatrix[startRow][i] != 0) {
								seatMatrix[startRow][i] = modifyMethod;
								seatAgeMatrix[startRow][i] = modifyMethod;
								successfulAdd++;
							}
							else if (seatAgeMatrix[startRow][i] == 0) {
								seatMatrix[startRow][i] = modifyMethod;
								seatAgeMatrix[startRow][i] = ageIndex;
								successfulAdd++;
							}
							else if (seatAgeMatrix[startRow][i] == ageIndex && modifyMethod == 2) {
								seatMatrix[startRow][i] = modifyMethod;
								successfulAdd++;
							}
							else {
								failureAdd++;
							}
						}
						tscui::modifyFileData(filePath, eventFileName, performanceDetail, user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
						seatPass = true;
					}
					else {
						seatPass = false;
						currentOutputText = "  Invalid input format entered, please try again";
						currentOutputText = tscui::modifyOutputText(currentOutputText);
						std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						std::cout << "\x1b[6A";
					}
				}
				else if (multipleModifyMethod == '|') {
					if (startColumn == endColumn) {
						for (int i = startRow; i < endRow; i++) {
							if (modifyMethod == 0 && seatAgeMatrix[i][startColumn] != 0) {
								seatMatrix[i][startColumn] = modifyMethod;
								seatAgeMatrix[i][startColumn] = modifyMethod;
								successfulAdd++;
							}
							else if (seatAgeMatrix[i][startColumn] == 0) {
								seatMatrix[i][startColumn] = modifyMethod;
								seatAgeMatrix[i][startColumn] = ageIndex;
								successfulAdd++;
							}
							else if (seatAgeMatrix[i][startColumn] == ageIndex && modifyMethod == 2) {
								seatMatrix[i][startColumn] = modifyMethod;
								successfulAdd++;
							}
							else {
								failureAdd++;
							}
						}
						tscui::modifyFileData(filePath, eventFileName, performanceDetail, user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
						seatPass = true;
					}
					else {
						seatPass = false;
						currentOutputText = "  Invalid input format entered, please try again";
						currentOutputText = tscui::modifyOutputText(currentOutputText);
						std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
						std::this_thread::sleep_for(std::chrono::milliseconds(1000));
						std::cout << "\x1b[6A";
					}
				}
				else {
					seatPass = false;
					currentOutputText = "  Invalid input format entered, please try again";
					currentOutputText = tscui::modifyOutputText(currentOutputText);
					std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
					std::cout << "\x1b[6A";
				}
			}
			else {
				currentOutputText = "  Invalid input format entered, please try again";
				currentOutputText = tscui::modifyOutputText(currentOutputText);
				std::cout << "\x1b[A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				std::cout << "\x1b[6A";
			}
		}
	} while (!(seatPass));
	currentOutputChar = 45;
	std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
	currentOutputText = "Results";
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << currentOutputDeco << "  " << "\x1b[4m" << currentOutputText << "\x1b[24m" << std::setw(UISIZE - currentOutputText.size() - 2) << std::setfill(' ') << "" << normalDeco << std::endl;
	currentOutputDeco = "\x1b[92;40m";
	currentOutputText = "  Successful modified: " + std::to_string(successfulAdd);
	currentOutputText = tscui::modifyOutputText(currentOutputText);
	std::cout << currentOutputDeco << "" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
	currentOutputDeco = "\x1b[91;40m";
	currentOutputText = "  Failure modified: " + std::to_string(failureAdd);
	std::cout << currentOutputDeco << "" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
	currentOutputDeco = "\x1b[97;40m";
	currentOutputChar = 45;
	std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
	currentOutputText = "Returning to modify menu";
	for (int i = 0; i <= 3; i++) {
		std::cout << "\x1b[92;40m" << "  " << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << "\x1b[0m" << std::endl;
		currentOutputChar = 45;
		std::cout << "\x1b[97;40m" << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << "\x1b[0m" << std::endl;
		currentOutputText = currentOutputText + ".";
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "\x1b[2A";
	}
	std::cout << std::endl << std::endl;
	endScene();
	tscui::systemModifyEventDetail(eventFileName, performanceDetail, user, password, position);
}

void tscui::systemEventModifyAgePrice(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) {
	int currentOutputChar = 124;
	int decimalAmount = 0;
	double newPrice = 0.0;
	std::string currentOutputText = "", currentOutputDeco = "\x1b[97;40m", normalDeco = "\x1b[0m", cursorUIRewindSpace = "";
	std::string userInput = "";
	std::string filePath = "performanceDetailFolder\\" + eventFileName + ".txt";
	bool inputPass = false, canConvert = true;
	currentOutputChar = 45;
	for (int i = 0; i < 3; i++) {
		inputPass = false;
		while (!(inputPass)) {
			decimalAmount = 0;
			canConvert = true;
			if (i == 0) {
				currentOutputText = "  Enter new price for senior: RM";
			}
			else if (i == 1) {
				currentOutputText = "  Enter new price for adult: RM";

			}
			else if (i == 2) {
				currentOutputText = "  Enter new price for children: RM";
			}
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			cursorUIRewindSpace = std::to_string(currentOutputText.size());
			std::cout << currentOutputDeco << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
			std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
			std::cout << "\x1b[2A\x1b[" + cursorUIRewindSpace + "C" << currentOutputDeco;
			getline(std::cin, userInput);
			for (int j = 0; j < userInput.size(); j++) {
				if (userInput[j] == '.') {
					decimalAmount++;
					if (decimalAmount > 1) {
						canConvert = false;
						break;
					}
				}
				else if (!(userInput[j] >= '0' && userInput[j] <= '9')) {
					canConvert = false;
					break;
				}
			}
			if (canConvert) {
				newPrice = std::stod(userInput);
				agePriceList[i] = newPrice;
				inputPass = true;
			}
			else {
				currentOutputText = "  Invalid value entered, please try again";
				currentOutputText = tscui::modifyOutputText(currentOutputText);
				std::cout << "\x1b[2A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
			}
		}
	}
	tscui::modifyFileData(filePath, eventFileName, performanceDetail, user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
	std::cout << currentOutputDeco << std::setw(UISIZE) << std::setfill('-') << "" << normalDeco << std::endl;
	currentOutputText = "Successfully updated age price list, returning to modify menu";
	for (int i = 0; i <= 3; i++) {
		std::cout << "\x1b[92;40m" << "  " << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << "\x1b[0m" << std::endl;
		currentOutputChar = 45;
		std::cout << "\x1b[97;40m" << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << "\x1b[0m" << std::endl;
		currentOutputText = currentOutputText + ".";
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "\x1b[2A";
	}
	std::cout << std::endl << std::endl;
	endScene();
	tscui::systemModifyEventDetail(eventFileName, performanceDetail, user, password, position);
}

void tscui::systemEventModifyRowPrice(std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) {
	int currentOutputChar = 124;
	int decimalAmount = 0;
	double newPrice = 0.0;
	std::string currentOutputText = "", currentOutputDeco = "\x1b[97;40m", normalDeco = "\x1b[0m", cursorUIRewindSpace = "";
	std::string userInput = "";
	std::string filePath = "performanceDetailFolder\\" + eventFileName + ".txt";
	bool inputPass = false, canConvert = true;
	currentOutputChar = 45;
	for (int i = 0; i < rowPrice.size(); i++) {
		inputPass = false;
		while (!(inputPass)) {
			decimalAmount = 0;
			canConvert = true;
			currentOutputText = "Enter new price for row ";
			currentOutputText += char(i + 65);
			currentOutputText = currentOutputText + ": RM";
			currentOutputText = tscui::modifyOutputText(currentOutputText);
			cursorUIRewindSpace = std::to_string(currentOutputText.size());
			std::cout << currentOutputDeco << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
			std::cout << currentOutputDeco << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << normalDeco << std::endl;
			std::cout << "\x1b[2A\x1b[" + cursorUIRewindSpace + "C" << currentOutputDeco; //cite the new things you did, the learn thigns you learned
			getline(std::cin, userInput);
			for (int j = 0; j < userInput.size(); j++) {
				if (userInput[j] == '.') {
					decimalAmount++;
					if (decimalAmount > 1) {
						canConvert = false;
						break;
					}
				}
				else if (!(userInput[j] >= '0' && userInput[j] <= '9')) {
					canConvert = false;
					break;
				}
			}
			if (canConvert) {
				newPrice = std::stod(userInput);
				rowPrice[i] = newPrice;
				inputPass = true;
			}
			else {
				currentOutputText = "  Invalid value entered, please try again";
				currentOutputText = tscui::modifyOutputText(currentOutputText);
				std::cout << "\x1b[2A" << currentOutputDeco << "\x1b[91m" << currentOutputText << std::setw(UISIZE - currentOutputText.size()) << std::setfill(' ') << "" << normalDeco << std::endl;
			}
		}
	}
	tscui::modifyFileData(filePath, eventFileName, performanceDetail, user, password, position, seatRowAmount, seatColumnAmount, agePriceList, rowPrice, seatMatrix, seatAgeMatrix);
	std::cout << currentOutputDeco << std::setw(UISIZE) << std::setfill('-') << "" << normalDeco << std::endl;
	currentOutputText = "Successfully updated row price list, returning to modify menu";
	for (int i = 0; i <= 3; i++) {
		std::cout << "\x1b[92;40m" << "  " << currentOutputText << "\x1b[97m" << std::setfill(' ') << std::setw(UISIZE - currentOutputText.size() - 2) << "" << "\x1b[0m" << std::endl;
		currentOutputChar = 45;
		std::cout << "\x1b[97;40m" << std::setfill(char(currentOutputChar)) << std::setw(UISIZE) << "" << "\x1b[0m" << std::endl;
		currentOutputText = currentOutputText + ".";
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "\x1b[2A";
	}
	std::cout << std::endl << std::endl;
	endScene();
	tscui::systemModifyEventDetail(eventFileName, performanceDetail, user, password, position);
}

void tscui::modifyFileData(std::string filePath, std::string eventFileName, performanceNameData performanceDetail, std::string user, std::string password, std::string position, int seatRowAmount, int seatColumnAmount, double agePriceList[], std::vector<double> rowPrice, std::vector<std::vector<int>> seatMatrix, std::vector<std::vector<int>> seatAgeMatrix) {
	std::fstream fileData;
	fileData.open(filePath, std::ios_base::out);
	fileData << seatRowAmount << " " << seatColumnAmount << " " << "\n";
	for (int i = 0; i < 3; i++) {
		fileData << agePriceList[i] << " ";
		if (i == 2) {
			fileData << "\n";
		}
	}
	for (int i = 0; i < rowPrice.size(); i++) {
		fileData << rowPrice[i] << " ";
		if (i == size(rowPrice) - 1) {
			fileData << "\n";
		}
	}
	for (int i = 0; i < seatRowAmount; i++) {
		for (int j = 0; j < seatColumnAmount; j++) {
			fileData << seatMatrix[i][j] << " ";
		}
		fileData << "\n";
	}
	fileData << "\n";
	for (int i = 0; i < seatRowAmount; i++) {
		for (int j = 0; j < seatColumnAmount; j++) {
			fileData << seatAgeMatrix[i][j] << " ";
		}
		fileData << "\n";
	}
	fileData.close();
}