#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <QMessageBox>

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

void showErrorMessage(std::string text);

}
#endif // CONSTANTS_H
