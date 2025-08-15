#include "../../include/core/economyviewer.h"
#include "../../include/core/constants.h"
#include <QMessageBox>

namespace core{

EconomyViewer::EconomyViewer() {}

int EconomyViewer::run(int argc, char **argv) {
    QApplication app (argc, argv);
    window.reset(new QFrame());
    window->show();

    transactionsView.reset(new views::Transactions(window.get()));
    QObject::connect(transactionsView.get(), &views::Transactions::openNewTransactionsView, this, &EconomyViewer::openNewTransactionsView);
    QObject::connect(transactionsView.get(), &views::Transactions::openAccountsView, this, &EconomyViewer::openAccountsView);
    transactionsView->openView();
    currentView = ViewNames::TRANSACTIONS;

    newTransactionsView.reset(new views::NewTransactions(window.get()));
    QObject::connect(newTransactionsView.get(), &views::NewTransactions::openTransactionsView, this, &EconomyViewer::openTransactionsView);
    QObject::connect(newTransactionsView.get(), &views::NewTransactions::addNewTransactions, this, &EconomyViewer::addNewTransactions);
    newTransactionsView->hide();

    accountsView.reset(new views::Accounts(window.get()));
    QObject::connect(accountsView.get(), &views::Accounts::openTransactionsView, this, &EconomyViewer::openTransactionsView);
    QObject::connect(accountsView.get(), &views::Accounts::updateAccounts, this, &EconomyViewer::updateAccounts);
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
    transactionsView->openView(transactions);
    currentView = ViewNames::TRANSACTIONS;
}

void EconomyViewer::openTransactionsView(bool useCoreTransactionVector){
    closeCurrentView();
    if(useCoreTransactionVector)
        transactionsView->openView(this->transactions);
    else
        transactionsView->openView();
    currentView = ViewNames::TRANSACTIONS;
}

void EconomyViewer::openNewTransactionsView(bool loadFileDialog){
    if(newTransactionsView->openView(accounts, loadFileDialog)){
        closeCurrentView();
        currentView = ViewNames::NEW_TRANSACTIONS;
    }
}

void EconomyViewer::openAccountsView(bool useCoreAccountsVector){
    closeCurrentView();
    if(useCoreAccountsVector)
        accountsView->openView(this->accounts);
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
        transactions.push_back(trans);
}

void EconomyViewer::updateAccounts(std::vector<Account>& updatedAccounts){
    accounts = updatedAccounts;
}
}
