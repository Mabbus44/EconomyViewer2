#include "../../include/views/transactions.h"
#include "../../include/core/constants.h"
#include <QMessageBox>
#include <QSizePolicy>
#include <QtWidgets>

namespace views{

Transactions::Transactions() {
    createViewElements();
}

Transactions::Transactions(QWidget* parent):QFrame(parent){
    createViewElements();
}

bool Transactions::openView(){
    this->show();
    return true;
}

bool Transactions::openView(std::vector<core::Transaction>& transactions){
    tblTransactions.setRowCount(transactions.size());
    tblTransactions.setColumnCount(core::TransactionColumns::COLUMN_COUNT);
    tblTransactions.setHorizontalHeaderLabels({"Date","Transfer amount","Balance","Comment","Account"});
    int row = 0;
    for(core::Transaction& transaction: transactions){
        tblTransactions.setItem(row, core::TransactionColumns::TRANSACTION_DATE, new QTableWidgetItem(transaction.getTransactionDateAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::TRANSACTION_AMOUNT, new QTableWidgetItem(transaction.getTransactionAmountAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::BALANCE, new QTableWidgetItem(transaction.getBalanceAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::DESCRIPTION, new QTableWidgetItem(transaction.getDescriptionAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::ACCOUNT, new QTableWidgetItem(transaction.getFromAccountAsString().c_str()));
        row++;
    }
    tblTransactions.resizeColumnsToContents();
    this->show();
    return true;
}

void Transactions::createViewElements(){
    btnAddTransactions.setParent(this);
    btnAddTransactions.setText("Add new transactions");
    btnAddTransactions.setToolTip("Loads new transactions from a file (usually an export from an internet banks account history)");
    btnAddTransactions.show();

    btnManageAccounts.setParent(this);
    btnManageAccounts.setText("Manage accounts");
    btnManageAccounts.setToolTip("Add, remove or edit bank accounts");
    btnManageAccounts.show();

    tblTransactions.setParent(this);
    tblTransactions.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    tblTransactions.setFixedHeight(300);
    tblTransactions.setFixedWidth(500);
    tblTransactions.show();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(&tblTransactions);
    layout->addWidget(&btnAddTransactions);
    layout->addWidget(&btnManageAccounts);
    this->setLayout(layout);

    this->show();
    QObject::connect(&btnAddTransactions, &QPushButton::clicked, this, &Transactions::btnAddTransactionsClick);
    QObject::connect(&btnManageAccounts, &QPushButton::clicked, this, &Transactions::btnManageAccountsClick);
}

void Transactions::btnAddTransactionsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    emit openNewTransactionsView(true);
}

void Transactions::btnManageAccountsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    emit openAccountsView(true);
}
}
