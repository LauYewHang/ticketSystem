#include "integervalidate.h"
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <climits>

const std::string MAX_INTEGER_STRING = "2147483647";

int intvalidate::identifyDigitValue(int index, int integerLength, char value) {
	int results = 1, realValue = 0;

	if (value != '0') {
		switch (value) {
			case '1':
				realValue = 1;
				break;
			case '2':
				realValue = 2;
				break;
			case '3':
				realValue = 3;
				break;
			case '4':
				realValue = 4;
				break;
			case '5':
				realValue = 5;
				break;
			case '6':
				realValue = 6;
				break;
			case '7':
				realValue = 7;
				break;
			case '8':
				realValue = 8;
				break;
			case '9':
				realValue = 9;
				break;
		}
		for (int i = 0; i < integerLength - 1 - index; i++) {
			results = results * 10;
		}
		return results * realValue;
	}
	else {
		return realValue;
	}
}

int intvalidate::strToInt(std::string strNumber, bool validateStrInput, bool allowDecimal, std::string round) {
	bool isNegative = false, hasDecimal = false, decimalValueNextToPoint = false, canConvert = true;
	int errorCode = 0, decimalIndex = 0, decimalIndex2 = 0, decimalValue = 0, decimalAmount = 0, nonIntCharIndex = 0, realInteger = 0;
	char nonIntChar = '0';
	std::string realStrNumber = "";
	if (strNumber[0] == '-') {
		isNegative = true;
		for (int i = 1; i < strNumber.size(); i++) {
			realStrNumber = realStrNumber + strNumber[i];
		}
	}
	else {
		realStrNumber = strNumber;
	}
	if (validateStrInput) {
		for (int i = 0; i < realStrNumber.size() && canConvert; i++) {
			if (!(realStrNumber[i] >= '0' && realStrNumber[i] <= '9')) {
				if (realStrNumber[i] == '.' && allowDecimal) {
					if (decimalAmount == 0) {
						decimalIndex = i;
						decimalAmount += 1;
					}
					else {
						decimalIndex2 = i;
						canConvert = false;
						errorCode = 1;
					}
				}
				else {
					nonIntCharIndex = i;
					nonIntChar = realStrNumber[i];
					canConvert = false;
					errorCode = 2;
				}
			}
		}
		if (canConvert) {
			if (decimalAmount == 0) {
				if (realStrNumber.size() > 10) {
					canConvert = false;
					errorCode = 3;
				}
			}
			else {
				if (decimalIndex > 10) {
					canConvert = false;
					errorCode = 3;
				}
			}
		}
		if (canConvert) {
			for (int i = 0; i < realStrNumber.size() && canConvert && ((realStrNumber.size() == MAX_INTEGER_STRING.size()) || decimalIndex == MAX_INTEGER_STRING.size()); i++) {
				if (realStrNumber[i] > MAX_INTEGER_STRING[i]) {
					canConvert = false;
					errorCode = 3;
				}
			}
		}
	}
	if (canConvert) {
		if (decimalAmount == 1) {
			if (!((decimalIndex + 1) >= realStrNumber.size())) {
				for (int i = decimalIndex + 1; i < realStrNumber.size(); i++) {
					if (realStrNumber[i] != '0') {
						decimalValue = intvalidate::identifyDigitValue(1, 1, realStrNumber[i]);
						if (i == decimalIndex + 1) {
							decimalValueNextToPoint = true;
						}
						break;
					}
				}
			}
			else {
				decimalValue = 0;
			}
			for (int i = 0; i < decimalIndex; i++) {
				realInteger = realInteger + intvalidate::identifyDigitValue(i, decimalIndex, realStrNumber[i]);
			}
			if (!(isNegative)) {
				if (round == "round") {
					if (decimalValueNextToPoint && decimalValue >= 5) {
						if (realInteger + 1 <= INT_MAX) {
							realInteger += 1;
						}
					}	
				}
				else if (round == "ceil") {
					if (decimalValue != 0) {
						if (realInteger + 1 <= INT_MAX) {
							realInteger += 1;
						}
					}
				}
			}
			else {
				if (round == "round") {
					if (decimalValueNextToPoint && decimalValue >= 5) {
						if (realInteger + 1 <= INT_MAX) {
							realInteger += 1;
						}
					}
				}
				else if (round == "floor") {
					if (decimalValue != 0) {
						if (realInteger + 1 <= INT_MAX) {
							realInteger += 1;
						}
					}
				}
			}
		}
		else {
			for (int i = 0; i < realStrNumber.size(); i++) {
				realInteger = realInteger + intvalidate::identifyDigitValue(i, realStrNumber.size(), realStrNumber[i]);
			}
		}
		if (!(isNegative)) {
			return realInteger;
		}
		else {
			return -realInteger;
		}
	}
	else {
		if (isNegative) {
			decimalIndex += 1;
			decimalIndex2 += 1;
			nonIntCharIndex += 1;
		}
		if (errorCode == 1) {
			std::cerr << "Error: Detected input string has more than one decimal point. Cannot convert value with more than one decimal point." << std::endl;
			std::cerr << "Input value: " << strNumber << std::endl << "First decimal point position: " << decimalIndex + 1 << std::endl << "Second decimal point position: " << decimalIndex2 + 1 << std::endl;
		}
		else if (errorCode == 2) {
			std::cerr << "Error: Detected input string has non-numeric character. Cannot convert alphabet or special character into a numeric value." << std::endl;
			std::cerr << "Input value: " << strNumber << std::endl << "Non-numeric character position: " << nonIntCharIndex + 1 << std::endl << "Non-numeric character detected: " << nonIntChar << std::endl;
		}
		else if (errorCode == 3) {
			if (isNegative) {
				std::cerr << "Error: The value intended to convert exceeded the minimum value for a variable of type int." << std::endl << "Minimum value: -" << MAX_INTEGER_STRING << std::endl;
			}
			else {
				std::cerr << "Error: The value intended to convert exceeded the maximum value for a variable of type int." << std::endl << "Maximum value: " << MAX_INTEGER_STRING << std::endl;
			}
			std::cerr << "Input value: " << strNumber << std::endl;
		}
		std::cerr << "Exit status: 1, returning 0" << std::endl;
		return 0;
		exit(1);
	}
}

int intvalidate::getInteger(std::string promptMessage, std::string errorMessage, bool promptNewline, bool errorNewline, void (*specialAction)()) {
	std::string userInput = "";
	bool pass = false, check = true;

	std::cout << promptMessage;
	if (promptNewline) {
		std::cout << std::endl;
	}
	while (!(pass)) {
		check = true;
		getline(std::cin, userInput);
		if (size(userInput) > 10) {
			check = false;
			std::cout << errorMessage;
			if (errorNewline) {
				std::cout << std::endl;
			}
			std::cout << promptMessage;
			if (promptNewline) {
				std::cout << std::endl;
			}
			specialAction();
		}
		if (size(userInput) == 0) {
			check = false;
			std::cout << errorMessage;
			if (errorNewline) {
				std::cout << std::endl;
			}
			std::cout << promptMessage;
			if (promptNewline) {
				std::cout << std::endl;
			}
			specialAction();
		}
		for (int i = 0; i < userInput.size() && check; i++) {
			if (!(userInput[i] >= '0' && userInput[i] <= '9')) {
				std::cout << errorMessage;
				if (errorNewline) {
					std::cout << std::endl;
				}
				std::cout << promptMessage;
				if (promptNewline) {
					std::cout << std::endl;
				}
				specialAction();
				check = false;
				pass = false;
			}
			else {
				pass = true;
			}
		}
		if (size(userInput) == 10) {
			for (int i = 0; i < userInput.size() && check; i++) {
				if (userInput[i] > MAX_INTEGER_STRING[i]) {
					check = false;
					pass = false;
					std::cout << errorMessage;
					if (errorNewline) {
						std::cout << std::endl;
					}
					std::cout << promptMessage;
					if (promptNewline) {
						std::cout << std::endl;
					}
					specialAction();
				}
			}
		}
	}
	return intvalidate::strToInt(userInput);
}