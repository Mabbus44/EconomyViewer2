#include "../../include/core/economyviewer.h"
#include "../../include/core/constants.h"
#include <QMessageBox>

namespace core{

EconomyViewer::EconomyViewer() {
    /*TransactionGroup t1;
    t1.accountName("acc1");
    t1.accountNumber("num1");
    TransactionGroup t2;
    t2.accountName("acc2");
    t2.accountNumber("num2");
    std::list<MatchCondition> m;
    MatchCondition m1;
    MatchCondition m2;
    m.push_back(m1);
    m.push_back(m2);
    t1.matchConditions(m);
    _transactionGroups.push_back(t1);
    _transactionGroups.push_back(t2);*/
}

int EconomyViewer::run(int argc, char **argv) {
    QApplication app (argc, argv);
    window.reset(new QFrame());
    window->show();

    transactionsView.reset(new views::Transactions(window.get(), this));
    transactionsView->openView();
    currentView = ViewNames::TRANSACTIONS;

    newTransactionsView.reset(new views::NewTransactions(window.get(), this));
    newTransactionsView->hide();

    accountsView.reset(new views::Accounts(window.get(), this));
    accountsView->hide();

    transactionGroupsView.reset(new views::TransactionGroups(window.get(), this));
    transactionGroupsView->hide();

    matchConditionsView.reset(new views::MatchConditions(window.get(), this));
    matchConditionsView->hide();

    return app.exec();
}

void EconomyViewer::closeCurrentView(){
    switch(currentView){
    case ViewNames::TRANSACTIONS:
        transactionsView->hide();
        break;
    case ViewNames::NEW_TRANSACTIONS:
        newTransactionsView->hide();
        break;
    case ViewNames::MANAGE_ACCOUNTS:
        accountsView->hide();
        break;
    case ViewNames::TRANSACTIONS_GROUPS:
        transactionGroupsView->hide();
        break;
    case ViewNames::MATCH_CONDITIONS:
        matchConditionsView->hide();
        break;
    default:
        break;
    }
    currentView = ViewNames::NONE;
}

void EconomyViewer::openTransactionsViewAndAddTransactions(std::vector<Transaction>& transactions){
    closeCurrentView();
    transactionsView->openView(transactions, true);
    currentView = ViewNames::TRANSACTIONS;
}

void EconomyViewer::openTransactionsView(bool useCoreTransactionVector){
    closeCurrentView();
    if(useCoreTransactionVector)
        transactionsView->openView(_transactions);
    else
        transactionsView->openView();
    currentView = ViewNames::TRANSACTIONS;
}

void EconomyViewer::openNewTransactionsView(bool loadFileDialog){
    if(newTransactionsView->openView(_accounts, loadFileDialog)){
        closeCurrentView();
        currentView = ViewNames::NEW_TRANSACTIONS;
    }
}

void EconomyViewer::openAccountsView(bool useCoreAccountsVector){
    closeCurrentView();
    if(useCoreAccountsVector)
        accountsView->openView(this->_accounts);
    else
        accountsView->openView();
    currentView = ViewNames::MANAGE_ACCOUNTS;
}

void EconomyViewer::openTransactionGroupsView(bool useCoreTransactionGroupsVector){
    closeCurrentView();
    if(useCoreTransactionGroupsVector)
        transactionGroupsView->openView(_transactionGroups);
    else
        transactionGroupsView->openView();

    currentView = ViewNames::TRANSACTIONS_GROUPS;
}

void EconomyViewer::openTransactionGroupsView(std::map<unsigned int, core::TransactionGroup>& transactionGroups){
    closeCurrentView();
    transactionGroupsView->openView(transactionGroups, true);

    currentView = ViewNames::TRANSACTIONS_GROUPS;
}

void EconomyViewer::openMatchConditionsView(core::TransactionGroup& transactionGroup){
    closeCurrentView();
    matchConditionsView->openView(transactionGroup);
    currentView = ViewNames::MATCH_CONDITIONS;
}

void EconomyViewer::updateAccounts(std::vector<Account>& updatedAccounts){
    _accounts = updatedAccounts;
}

void EconomyViewer::updateTransactions(std::vector<Transaction>& updatedTransactions){
    _transactions.clear();
    for(Transaction& transaction: updatedTransactions){
        unsigned int id = transaction.getId();
        _transactions[id] = transaction;
    }
    core::Utils::showErrorMessage("Update core transactions, new size: " + std::to_string(_transactions.size()));
}

void EconomyViewer::updateTransactionGroups(std::map<unsigned int, core::TransactionGroup>& updatedTransactionGroups){
    _transactionGroups = updatedTransactionGroups;
}

unsigned int EconomyViewer::getUniqueTransactionId(){
    return _transactionId++;
}

unsigned int EconomyViewer::getUniqueTransactionGroupId(){
    return _transactionGroupId++;
}

TableRowState::TableRowState EconomyViewer::getTableRowState(core::Transaction& transaction){
    unsigned int id = transaction.getId();
    if(_transactions.count(id)){
        if(_transactions[id] == transaction)
            return TableRowState::UNCHANGED;
        else
            return TableRowState::CHANGED;
    }
    return TableRowState::NEW;
}

}
