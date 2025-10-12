#ifndef ECONOMYVIEWER_H
#define ECONOMYVIEWER_H

#include <QApplication>
#include <QFrame>
#include <map>
#include "../core/account.h"
#include "../core/constants.h"
#include "../core/transaction.h"
#include "../views/transactions.h"
#include "../views/newtransactions.h"
#include "../views/accounts.h"
#include "../views/transactionGroups.h"

namespace views{
class Transactions;
class NewTransactions;
class Accounts;
class TransactionGroups;
}

namespace core{

class EconomyViewer: public QObject
{
Q_OBJECT
public:
    EconomyViewer();
    int run(int argc, char **argv);
    void closeCurrentView();

    ViewNames currentView = ViewNames::NONE;
    QScopedPointer<views::Transactions> transactionsView;
    QScopedPointer<views::NewTransactions> newTransactionsView;
    QScopedPointer<views::Accounts> accountsView;
    QScopedPointer<views::TransactionGroups> transactionGroupsView;
    QScopedPointer<QFrame> window;
    void openTransactionsViewAndAddTransactions(std::vector<Transaction>& transactions);
    void openTransactionsView(bool useCoreTransactionVector = false);
    void openNewTransactionsView(bool loadFileDialog = false);
    void openAccountsView(bool useCoreAccountsVector = false);
    void openTransactionGroupsView();
    void addNewTransactions(std::vector<Transaction>& newTransactions);
    void updateAccounts(std::vector<Account>& updatedAccounts);
    void updateTransactions(std::vector<Transaction>& updatedTransactions);
    unsigned int getUniqueId();
    TransactionState getTransactionState(core::Transaction& transaction);
private:
    std::vector<Account> _accounts;
    std::map<unsigned int, Transaction> _transactions;
    unsigned int _transactionId=1;
};

}
#endif // ECONOMYVIEWER_H
