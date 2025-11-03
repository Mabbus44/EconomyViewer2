#ifndef ECONOMYVIEWER_H
#define ECONOMYVIEWER_H

#include <QApplication>
#include <QFrame>
#include <map>
#include "../core/account.h"
#include "../core/constants.h"
#include "../core/transaction.h"
#include "../core/transactionGroup.h"
#include "../views/transactions.h"
#include "../views/newtransactions.h"
#include "../views/accounts.h"
#include "../views/transactionGroups.h"
#include "../views/matchConditions.h"

namespace views{
class Transactions;
class NewTransactions;
class Accounts;
class TransactionGroups;
class MatchConditions;
}

namespace core{

class EconomyViewer: public QObject
{
Q_OBJECT
public:
    EconomyViewer();
    int run(int argc, char **argv);
    void closeCurrentView();

    ViewNames::ViewNames currentView = ViewNames::NONE;
    QScopedPointer<views::Transactions> transactionsView;
    QScopedPointer<views::NewTransactions> newTransactionsView;
    QScopedPointer<views::Accounts> accountsView;
    QScopedPointer<views::TransactionGroups> transactionGroupsView;
    QScopedPointer<views::MatchConditions> matchConditionsView;
    QScopedPointer<QFrame> window;

    void openTransactionsViewAndAddTransactions(std::vector<Transaction>& transactions);
    void openTransactionsView(bool useCoreTransactionVector = false);
    void openNewTransactionsView(bool loadFileDialog = false);
    void openAccountsView(bool useCoreAccountsVector = false);
    void openTransactionGroupsView(bool useCoreTransactionGroupsVector = false);
    void openTransactionGroupsView(std::map<unsigned int, core::TransactionGroup>& transactionGroups);
    void openMatchConditionsView(core::TransactionGroup& transactionGroup);

    void updateAccounts(std::vector<Account>& updatedAccounts);
    void updateTransactions(std::vector<Transaction>& updatedTransactions);
    void updateTransactionGroups(std::map<unsigned int, core::TransactionGroup>& updatedTransactionGroups);
    unsigned int getUniqueTransactionId();
    unsigned int getUniqueTransactionGroupId();
    TableRowState::TableRowState getTableRowState(core::Transaction& transaction);

private:
    std::vector<Account> _accounts;
    std::map<unsigned int, Transaction> _transactions;
    std::map<unsigned int, core::TransactionGroup> _transactionGroups;
    unsigned int _transactionId=1;
    unsigned int _transactionGroupId=1;
};

}
#endif // ECONOMYVIEWER_H
