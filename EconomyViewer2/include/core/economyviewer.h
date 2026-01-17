#ifndef ECONOMYVIEWER_H
#define ECONOMYVIEWER_H

#include <QApplication>
#include <QFrame>
#include <QStackedLayout>
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
#include "../views/graph.h"

namespace views{
class Transactions;
class NewTransactions;
class Accounts;
class TransactionGroups;
class MatchConditions;
class Graph;
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
    QScopedPointer<views::Graph> graphView;
    QScopedPointer<QStackedLayout> layout;
    QScopedPointer<QFrame> window;

    void openTransactionsViewAndAddTransactions(std::vector<Transaction>& transactions);
    void openTransactionsView(bool useCoreTransactionVector = false);
    void openNewTransactionsView(bool loadFileDialog = false);
    void openAccountsView(bool useCoreAccountsVector = false);
    void openTransactionGroupsView(bool useCoreTransactionGroupsVector = false);
    void openTransactionGroupsView(std::map<unsigned int, core::TransactionGroup>& transactionGroups);
    void openMatchConditionsView(core::TransactionGroup& transactionGroup);
    void openGraphView();

    void updateAccounts(std::vector<Account>& updatedAccounts);
    void updateTransactions(std::vector<Transaction>& updatedTransactions);
    void updateTransactionGroups(std::map<unsigned int, core::TransactionGroup>& updatedTransactionGroups);
    bool saveToFile(std::string fileName);
    bool loadFromFile(std::string fileName);
    bool fromJson(JsonNode node);
    void clear();
    const std::map<unsigned int, Transaction>& getTransactionsRef() {return _transactions;}

    unsigned int getUniqueTransactionId();
    unsigned int getUniqueTransactionGroupId();
    TableRowState::TableRowState getTransactionTableRowState(core::Transaction& transaction);
    TableRowState::TableRowState getTransactionGroupTableRowState(core::TransactionGroup& transactionGroup);
    std::map<unsigned int, core::TransactionGroup>& transactionGroupsRef() {return _transactionGroups;};

private:
    std::vector<Account> _accounts;
    std::map<unsigned int, Transaction> _transactions;
    std::map<unsigned int, core::TransactionGroup> _transactionGroups;
    unsigned int _transactionId=1;
    unsigned int _transactionGroupId=1;
};

}
#endif // ECONOMYVIEWER_H
