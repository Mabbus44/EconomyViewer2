#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <QMessageBox>
#include <ctime>

namespace core{

enum ViewNames{
    NONE,
    TRANSACTIONS,
    NEW_TRANSACTIONS,
    MANAGE_ACCOUNTS,
    TRANSACTIONS_GROUPS,
    EDIT_TRANSACTION_GROUP,
    CHANGE_TRANSACTIONS
};

enum TransactionColumns{
    TRANSACTION_DATE = 0,
    TRANSACTION_AMOUNT = 1,
    BALANCE = 2,
    DESCRIPTION = 3,
    ACCOUNT = 4,
    ID = 5,
    COLUMN_COUNT = 6,
};

enum TransactionState{
    UNCHANGED = 0,
    CHANGED = 1,
    NEW = 2
};

enum DateComp{
    EARLIER = 0,
    SAME_TIME = 1,
    LATER = 2
};

class Utils{
public:
    Utils(){};
    ~Utils(){};
    static void showErrorMessage(std::string text);
    static std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);
    static bool isDate(std::string str);
    static bool isNum(std::string str);
    static std::tm toDate(std::string str);
    static double toNum(std::string str);
    static std::vector<std::tuple<int, int>> sortKeysByVal(std::map<int, int>& inputMap);
    static DateComp compareDates(std::tm dateA, std::tm dateB);
};


}
#endif // CONSTANTS_H
