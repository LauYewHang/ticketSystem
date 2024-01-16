#ifndef __INTEGERVALIDATE_H_INCLUDED__
#define __INTEGERVALIDATE_H_INCLUDED__

#include <string>
#include <cstring>

namespace intvalidate {
	int getInteger(std::string promptMessage = "", std::string errorMessage = "", bool promptNewline = false, bool errorNewline = false, void(*specialAction)() = [](){});
	int identifyDigitValue(int index, int integerLength, char value);
	int strToInt(std::string strNumber, bool validateStrInput = true, bool allowDecimal = true, std::string round = "floor");
}

#endif //integervalidate.h