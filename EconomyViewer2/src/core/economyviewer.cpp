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
    QObject::connect(transactionsView.get(), &views::Transactions::changedView, this, &EconomyViewer::openNewView);
    updateTransactionsTable();
    transactionsView->show();
    currentView = ViewNames::TRANSACTIONS;

    newTransactionsView.reset(new views::NewTransactions(window.get()));
    QObject::connect(newTransactionsView.get(), &views::NewTransactions::changedView, this, &EconomyViewer::openNewView);
    QObject::connect(newTransactionsView.get(), &views::NewTransactions::addNewTransactions, this, &EconomyViewer::addNewTransactions);

    newTransactionsView->hide();

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

void EconomyViewer::openNewView(ViewNames viewName){
    if(!window){
        Utils::showErrorMessage("Cannot open view without a window");
        return;
    }

    switch(viewName){
    case ViewNames::TRANSACTIONS:
        closeCurrentView();
        updateTransactionsTable();
        transactionsView->show();
        currentView = ViewNames::TRANSACTIONS;
        break;
    case ViewNames::NEW_TRANSACTIONS:
        if(newTransactionsView->loadTransactions()){
            closeCurrentView();
            newTransactionsView->show();
            currentView = ViewNames::NEW_TRANSACTIONS;
        }
        break;
    case ViewNames::MANAGE_ACCOUNTS:
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
}

void EconomyViewer::addNewTransactions(std::vector<Transaction>& newTransactions){
    for(Transaction& trans: newTransactions)
        transactions.push_back(trans);
}

void EconomyViewer::updateTransactionsTable(){
    // Populate the table
    transactionsView->tblTransactions.setRowCount(transactions.size());
    transactionsView->tblTransactions.setColumnCount(core::TransactionColumns::COLUMN_COUNT);
    transactionsView->tblTransactions.setHorizontalHeaderLabels({"Date","Transfer amount","Balance","Comment"});
    int row = 0;
    for(core::Transaction& transaction: transactions){
        transactionsView->tblTransactions.setItem(row, core::TransactionColumns::TRANSACTION_DATE, new QTableWidgetItem(transaction.getTransactionDateAsString().c_str()));
        transactionsView->tblTransactions.setItem(row, core::TransactionColumns::TRANSACTION_AMOUNT, new QTableWidgetItem(transaction.getTransactionAmountAsString().c_str()));
        transactionsView->tblTransactions.setItem(row, core::TransactionColumns::BALANCE, new QTableWidgetItem(transaction.getBalanceAsString().c_str()));
        transactionsView->tblTransactions.setItem(row, core::TransactionColumns::DESCRIPTION, new QTableWidgetItem(transaction.getDescriptionAsString().c_str()));
        row++;
    }
    transactionsView->tblTransactions.resizeColumnsToContents();
}

}
