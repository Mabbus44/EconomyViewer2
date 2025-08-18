#include "../../include/core/economyviewer.h"
#include "../../include/core/constants.h"
#include <QMessageBox>

namespace core{

EconomyViewer::EconomyViewer() {}

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
        break;
    case ViewNames::EDIT_TRANSACTION_GROUP:
        break;
    case ViewNames::CHANGE_TRANSACTIONS:
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
        transactionsView->openView(this->_transactions);
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

void EconomyViewer::openTransactionGroupsView(){
    closeCurrentView();
    transactionGroupsView->openView();
    currentView = ViewNames::TRANSACTIONS_GROUPS;
}

void EconomyViewer::addNewTransactions(std::vector<Transaction>& newTransactions){
    for(Transaction& trans: newTransactions)
        _transactions.push_back(trans);
}

void EconomyViewer::updateAccounts(std::vector<Account>& updatedAccounts){
    _accounts = updatedAccounts;
}

void EconomyViewer::updateTransactions(std::vector<Transaction>& updatedTransactions){
    _transactions = updatedTransactions;
    core::Utils::showErrorMessage("Update core transactions, new size: " + std::to_string(_transactions.size()));
}

unsigned int EconomyViewer::getUniqueId(){
    return _transactionId++;
}
}
