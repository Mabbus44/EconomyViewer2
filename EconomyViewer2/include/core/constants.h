#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <QMessageBox>
#include <ctime>

namespace TransactionGroupColumns{ enum TransactionGroupColumns{
    GROUP_NAME,
    RULES,
    ID,
    STATUS,
    COLUMN_COUNT,
};}

namespace MatchConditionColumns{ enum MatchConditionColumns{
    FIELD,
    COMPARATOR,
    VALUE,
    ID,
    STATUS,
    COLUMN_COUNT,
};}

namespace ViewNames{ enum ViewNames{
    NONE,
    TRANSACTIONS,
    NEW_TRANSACTIONS,
    MANAGE_ACCOUNTS,
    TRANSACTIONS_GROUPS,
    MATCH_CONDITIONS,
    GRAPH
};}

namespace TransactionColumns{ enum TransactionColumns{
    TRANSACTION_DATE,
    TRANSACTION_AMOUNT,
    BALANCE,
    DESCRIPTION,
    ACCOUNT,
    GROUP,
    ID,
    STATUS,
    COLUMN_COUNT,
};}


namespace TableRowState{ enum TableRowState{
    UNCHANGED,
    CHANGED,
    NEW,
    ERROR
};}

namespace DateComp{ enum DateComp{
    EARLIER,
    SAME_TIME,
    LATER
};}

namespace QtRoles{ enum QtRoles
{
    Row = Qt::UserRole + 1,
    Col
};
}

namespace CompareType{ enum CompareType{
    // For strings
    CONTAINS,
    STARTS_WITH,
    ENDS_WITH,
    // For integers or dates
    GRATER_THAN,
    LESS_THAN,
    GRATER_OR_EQUAL_TO,
    LESS_OR_EQUAL_TO,
    // For all
    EQUALS
};}

namespace CompareTypeStr{
    extern const char* CONTAINS;
    extern const char* STARTS_WITH;
    extern const char* ENDS_WITH;
    extern const char* GRATER_THAN;
    extern const char* LESS_THAN;
    extern const char* GRATER_OR_EQUAL_TO;
    extern const char* LESS_OR_EQUAL_TO;
    extern const char* EQUALS;
}

namespace CompareProperty{ enum CompareProperty{
    TRANSACTION_DATE_PROPERTY,
    TRANSACTION_AMOUNT_PROPERTY,
    BALANCE_PROPERTY,
    DESCRIPTION_PROPERTY,
    ACCOUNT_NAME_PROPERTY
};}

namespace ComparePropertyStr{
    extern const char* TRANSACTION_DATE_PROPERTY;
    extern const char* TRANSACTION_AMOUNT_PROPERTY;
    extern const char* BALANCE_PROPERTY;
    extern const char* DESCRIPTION_PROPERTY;
    extern const char* ACCOUNT_NAME_PROPERTY;
}

namespace core{

class Utils{
public:
    Utils(){};
    ~Utils(){};

    static void showErrorMessage(std::string text);

    static std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);
    static std::string addLeadingZeroes(int minDigitCount, int num);
    static bool isEmpty(std::string str);

    static bool isDate(std::string str);
    static bool isNum(std::string str);

    static std::tm toDate(std::string str);
    static std::string toString(std::tm date);
    static std::string toString(bool b);
    static std::string toLower(std::string str);
    static double toNum(std::string str);
    static int toInt(std::string str);
    static unsigned int toUInt(std::string str);
    static bool toBool(std::string str);

    static std::vector<std::tuple<int, int>> sortKeysByVal(std::map<int, int>& inputMap);

    static DateComp::DateComp compareDates(std::tm dateA, std::tm dateB);
};

}
#endif // CONSTANTS_H
