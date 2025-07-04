#include "../../include/core/constants.h"

namespace core{

void Utils::showErrorMessage(std::string text){
    QMessageBox messageBox;
    messageBox.critical(0,"Error", text.c_str());
    messageBox.setFixedSize(500,200);
};

std::vector<std::string> Utils::splitString(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    std::string subStr = s;
    while ((pos = subStr.find(delimiter)) != std::string::npos) {
        token = subStr.substr(0, pos);
        tokens.push_back(token);
        subStr.erase(0, pos + delimiter.length());
    }
    tokens.push_back(subStr);

    return tokens;
}

bool Utils::isDate(std::string str){
    int numberGroup = 0;
    int digitCount = 0;
    for(char& c:str){
        if((int)c >= 48 && (int)c <= 57){
            digitCount++;
            if(numberGroup == 0 && digitCount > 4)  // Max 4 digits in a year
                return false;
            if(numberGroup > 0 && digitCount > 2)   // Max 2 digits in a month or a day
                return false;
        }else{
            numberGroup++;
            if(numberGroup == 1 && digitCount != 2 && digitCount != 4)  // Finished year must have 2 or 4 digits
                return false;
            if(numberGroup > 1 && digitCount != 2)  // Finished month or day must have 2 digits
                return false;
            if(numberGroup >= 3)    // Year month and day finnished means we have a valid date
                return true;
            digitCount = 0;
        }
    }
    if(numberGroup == 2 && digitCount == 2) // If string ended after day
        numberGroup = 3;
    return numberGroup == 3;    // Check that we got both year, month and day
}

bool Utils::isNum(std::string str){
    bool firstChar = true;
    bool firstDigit = false;
    bool decimalSign = false;
    for(char& c:str){
        if(firstChar && c == '-'){} // First char can be negative
        else{
            if(firstDigit && !decimalSign && (c == '.' || c == ',')){
                decimalSign = true; // One decimal sign allowed after first digit
            }else{
                if((int)c >= 48 && (int)c <= 57){
                    firstDigit = true;
                }else{
                    return false;   // If not leading '-' or valid decimal sign, only numbers allowed
                }
            }
        }
        firstChar = false;
    }
    return firstDigit;
}

std::tm Utils::toDate(std::string str){
    std::tm ret;
    int numberGroup = 0;
    int digitCount = 0;
    std::string digits = "";
    ret.tm_hour = 0;
    ret.tm_min = 0;
    ret.tm_sec = 0;
    ret.tm_isdst = 0;       // No summer time (only dates here annyway, no time)

    for(char& c:str){
        if((int)c >= 48 && (int)c <= 57){
            digitCount++;
            digits = digits + c;
            if(numberGroup == 0 && digitCount > 4)  // Max 4 digits in a year
                break;
            if(numberGroup > 0 && digitCount > 2)   // Max 2 digits in a month or a day
                break;
        }else{
            numberGroup++;
            if(numberGroup == 1 && digitCount != 2 && digitCount != 4)  // Finished year must have 2 or 4 digits
                break;
            if(numberGroup > 1 && digitCount != 2)  // Finished month or day must have 2 digits
                break;
            if(numberGroup == 1)
                ret.tm_year = toNum(digits) - 1900; // Number of years since 1900
            if(numberGroup == 2)
                ret.tm_mon = toNum(digits) - 1; // Number of months since January
            if(numberGroup == 3)
                ret.tm_mday = toNum(digits);

            if(numberGroup >= 3)    // Year month and day finnished means we have a valid date
                break;
            digitCount = 0;
            digits = "";
        }
    }
    if(numberGroup == 2 && digitCount == 2){    // If string ended after day
        ret.tm_mday = toNum(digits);
    }
    return ret;
}

double Utils::toNum(std::string str){
    double ret = 0;
    double decimal = 1;
    bool negative = false;
    bool firstChar = true;
    bool firstDigit = false;
    bool decimalSign = false;
    for(char& c:str){
        if(firstChar && c == '-'){
            negative = true;
            firstChar = true;
            continue;
        }
        firstChar = true;
        if(firstDigit && !decimalSign && (c == '.' || c == ',')){
            decimalSign = true;
            continue;
        }
        if((int)c >= 48 && (int)c <= 57){
            firstDigit = true;
            if(decimalSign){
                decimal /= 10;
                ret += decimal * ((int)c -48);
            }else{
                ret *= 10;
                ret += (int)c -48;
            }
        }
        else{
            return 0;   // If not leading '-' or valid decimal sign, only numbers allowed
        }
    }
    if(negative)
        ret *= -1;
    return ret;
}

std::vector<std::tuple<int, int>> Utils::sortKeysByVal(std::map<int, int>& inputMap){
    // Sorts a number of key by their corresponding values (biggest first), and returns the result as a vector
    std::vector<std::tuple<int, int>> ret;
    for (std::map<int, int>::iterator itr = inputMap.begin(); itr != inputMap.end(); ++itr) {
        bool addedKey = false;
        for (std::vector<std::tuple<int,int>>::iterator itr2 = ret.begin(); itr2 != ret.end() && !addedKey; ++itr2) {
            if(itr->second >= std::get<1>(*itr2)){
                ret.insert(itr2, std::make_tuple(itr->first, itr->second));
                addedKey = true;
            }
        }
        if(!addedKey)
            ret.insert(ret.end(), std::make_tuple(itr->first, itr->second));
    }
    return ret;
}

}
