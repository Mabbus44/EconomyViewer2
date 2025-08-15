#ifndef ECONOMYVIEWER_H
#define ECONOMYVIEWER_H

#include <QApplication>
#include <QFrame>
#include "../core/account.h"
#include "../core/constants.h"
#include "../core/transaction.h"
#include "../views/transactions.h"
#include "../views/newtransactions.h"
#include "../views/accounts.h"
#include "../views/transactionGroups.h"

namespace core{

class EconomyViewer: public QObject
{
Q_OBJECT
public:
    EconomyViewer();
    int run(int argc, char **argv);
    void closeCurrentView();

    ViewNames currentView = ViewNames::NONE;
    std::vector<Account> accounts;
    std::vector<Transaction> transactions;
    QScopedPointer<views::Transactions> transactionsView;
    QScopedPointer<views::NewTransactions> newTransactionsView;
    QScopedPointer<views::Accounts> accountsView;
    QScopedPointer<views::TransactionGroups> transactionGroupsView;
    QScopedPointer<QFrame> window;
    void updateAccountsTable();
    void updateTransactionGroupsTable();

public slots:
    void openTransactionsViewAndAddTransactions(std::vector<Transaction>& transactions);
    void openTransactionsView(bool useCoreTransactionVector = false);
    void openNewTransactionsView(bool loadFileDialog = false);
    void openAccountsView(bool useCoreAccountsVector = false);
    void openTransactionGroupsView();
    void addNewTransactions(std::vector<Transaction>& newTransactions);
    void updateAccounts(std::vector<Account>& updatedAccounts);
};

}
#endif // ECONOMYVIEWER_H
