#include "../../include/core/economyviewer.h"
#include "../../include/views/transactions.h"
#include "../../include/core/constants.h"
#include <QMessageBox>
#include <QSizePolicy>
#include <QColor>
#include <QtWidgets>

namespace views{

Transactions::Transactions() {
    createViewElements();
}

Transactions::Transactions(QWidget* parent, core::EconomyViewer* core):QFrame(parent){
    _core = core;
    createViewElements();
}

bool Transactions::openView(){
    this->show();
    return true;
}

bool Transactions::openView(std::map<unsigned int, core::Transaction>& transactions, bool appendTransactions){
    std::vector<core::Transaction> transVector;
    for(auto const& p: transactions){
        transVector.push_back(p.second);
    }
    return openView(transVector, appendTransactions);
}

bool Transactions::openView(std::vector<core::Transaction>& transactions, bool appendTransactions){
    int row = 0;
    if(!appendTransactions || tblTransactions.rowCount() <= 0){
        tblTransactions.setRowCount(transactions.size());
        tblTransactions.setColumnCount(core::TransactionColumns::COLUMN_COUNT);
        tblTransactions.setHorizontalHeaderLabels({"Date","Transfer amount","Balance","Comment","Account", "id"});
        tblTransactions.hideColumn(core::TransactionColumns::ID);
    }else{
        row = tblTransactions.rowCount();
        tblTransactions.setRowCount(tblTransactions.rowCount() + transactions.size());
    }
    for(core::Transaction& transaction: transactions){
        tblTransactions.setItem(row, core::TransactionColumns::TRANSACTION_DATE, new QTableWidgetItem(transaction.getTransactionDateAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::TRANSACTION_AMOUNT, new QTableWidgetItem(transaction.getTransactionAmountAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::BALANCE, new QTableWidgetItem(transaction.getBalanceAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::DESCRIPTION, new QTableWidgetItem(transaction.getDescriptionAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::ACCOUNT, new QTableWidgetItem(transaction.getFromAccountAsString().c_str()));
        tblTransactions.setItem(row, core::TransactionColumns::ID, new QTableWidgetItem(transaction.getIdAsString().c_str()));
        for(int col=0; col<core::TransactionColumns::COLUMN_COUNT; col++)
            tblTransactions.item(row, col)->setBackground(Qt::green);
        row++;
    }
    tblTransactions.resizeColumnsToContents();
    this->show();
    return true;
}

bool Transactions::checkCellFormat(){
    int rowCount = tblTransactions.rowCount();
    int columnCount = tblTransactions.columnCount();
    if(columnCount < core::TransactionColumns::COLUMN_COUNT-1){
        core::Utils::showErrorMessage("Transaction table missing columns");
        return false;
    }
    bool ret = true;
    for(int row=0; row < rowCount; row++){
        std::string date = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->text().toStdString();
        std::string transactionAmount = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->text().toStdString();
        std::string balance = tblTransactions.item(row, core::TransactionColumns::BALANCE)->text().toStdString();
        std::string description = tblTransactions.item(row, core::TransactionColumns::DESCRIPTION)->text().toStdString();
        std::string id = tblTransactions.item(row, core::TransactionColumns::ID)->text().toStdString();
        if(core::Utils::isDate(date))
            tblTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->setBackground(Qt::white);
        else{
            tblTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->setBackground(Qt::red);
            ret = false;
        }
        if(core::Utils::isNum(transactionAmount))
            tblTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->setBackground(Qt::white);
        else{
            tblTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->setBackground(Qt::red);
            ret = false;
        }
        if(core::Utils::isNum(balance))
            tblTransactions.item(row, core::TransactionColumns::BALANCE)->setBackground(Qt::white);
        else{
            tblTransactions.item(row, core::TransactionColumns::BALANCE)->setBackground(Qt::red);
            ret = false;
        }
        if(core::Utils::isNum(id))
            tblTransactions.item(row, core::TransactionColumns::ID)->setBackground(Qt::white);
        else{
            tblTransactions.item(row, core::TransactionColumns::ID)->setBackground(Qt::red);
            ret = false;
        }
    }
    return ret;
}

bool Transactions::saveTransactionsToCore(){
    int rowCount = tblTransactions.rowCount();
    int columnCount = tblTransactions.columnCount();
    if(columnCount < core::TransactionColumns::COLUMN_COUNT-1){
        core::Utils::showErrorMessage("Transaction table missing columns");
        return false;
    }
    std::vector<core::Transaction> transactions;
    for(int row=0; row < rowCount; row++)
        transactions.push_back(rowToTransaction(row));
    _core->updateTransactions(transactions);
    return true;
}

core::Transaction Transactions::rowToTransaction(int row){
    std::string date = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->text().toStdString();
    auto a = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT);
    auto b = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->text();
    std::string transactionAmount = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->text().toStdString();
    std::string balance = tblTransactions.item(row, core::TransactionColumns::BALANCE)->text().toStdString();
    std::string description = tblTransactions.item(row, core::TransactionColumns::DESCRIPTION)->text().toStdString();
    std::string accountName = tblTransactions.item(row, core::TransactionColumns::ACCOUNT)->text().toStdString();
    std::string id = tblTransactions.item(row, core::TransactionColumns::ID)->text().toStdString();
    core::Transaction t;
    t.setTransactionDate(core::Utils::toDate(date));
    t.setTransactionAmount(core::Utils::toNum(transactionAmount));
    t.setBalance(core::Utils::toNum(balance));
    t.setDescription(description);
    t.setFromAccount(accountName);
    t.setId(core::Utils::toNum(id));
    return t;
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

    btnApplyChanges.setParent(this);
    btnApplyChanges.setText("Apply changes");
    btnApplyChanges.setToolTip("Applies all changes of transactions (does not save to file)");
    btnApplyChanges.show();

    tblTransactions.setParent(this);
    tblTransactions.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    tblTransactions.setFixedHeight(300);
    tblTransactions.setFixedWidth(500);
    tblTransactions.show();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(&tblTransactions);
    layout->addWidget(&btnAddTransactions);
    layout->addWidget(&btnManageAccounts);
    layout->addWidget(&btnApplyChanges);
    this->setLayout(layout);

    this->show();
    QObject::connect(&tblTransactions, &QTableWidget::cellChanged, this, &Transactions::tblTransactionsChanged);
    QObject::connect(&btnAddTransactions, &QPushButton::clicked, this, &Transactions::btnAddTransactionsClick);
    QObject::connect(&btnManageAccounts, &QPushButton::clicked, this, &Transactions::btnManageAccountsClick);
    QObject::connect(&btnApplyChanges, &QPushButton::clicked, this, &Transactions::btnApplyChangesClick);
}

void Transactions::tblTransactionsChanged(int row, int col){
    QBrush brush = tblTransactions.item(row, col)->background();
    core::Transaction transaction = rowToTransaction(row);
    core::TransactionState state = _core->getTransactionState(transaction);
    if(state == core::TransactionState::NEW && brush.color() != Qt::green){
        core::Utils::showErrorMessage("Change to green");
        //tblTransactions.item(row, col)->setBackground(Qt::green);
    }
    if(state == core::TransactionState::CHANGED && brush.color() != Qt::yellow){
        core::Utils::showErrorMessage("Change to yellow");
        //tblTransactions.item(row, col)->setBackground(Qt::yellow);
    }
    if(state == core::TransactionState::NEW && brush.color() != Qt::transparent){
        core::Utils::showErrorMessage("Change to transparent");
        //tblTransactions.item(row, col)->setBackground(Qt::transparent);
    }
}

void Transactions::btnAddTransactionsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    _core->openNewTransactionsView(true);
}

void Transactions::btnManageAccountsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    _core->openAccountsView(true);
}

void Transactions::btnApplyChangesClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    if(checkCellFormat()){
        saveTransactionsToCore();
    }
}

}
