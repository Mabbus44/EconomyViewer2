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
        tblTransactions.setHorizontalHeaderLabels({"Date","Transfer amount","Balance","Comment","Account", "Id", "Status"});
        tblTransactions.hideColumn(core::TransactionColumns::ID);
        tblTransactions.hideColumn(core::TransactionColumns::STATUS);
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
        for(int col=0; col<core::TransactionColumns::COLUMN_COUNT; col++){
            if(col != core::TransactionColumns::ID && col != core::TransactionColumns::STATUS)
                tblTransactions.item(row, col)->setBackground(Qt::green);
        }
        tblTransactions.setItem(row, core::TransactionColumns::ID, new QTableWidgetItem(transaction.getIdAsString().c_str()));
        core::TransactionState state;
        int errorCol = checkRowFormat(row);
        if(errorCol == -1)
            state = _core->getTransactionState(transaction);
        else
            state = core::TransactionState::ERROR;
        std::string itIsANumberRightQutionsmark = std::to_string(state).c_str();
        tblTransactions.setItem(row, core::TransactionColumns::STATUS, new QTableWidgetItem(std::to_string(state).c_str()));
        row++;
    }
    tblTransactions.resizeColumnsToContents();
    this->show();
    return true;
}

bool Transactions::checkAllCellsFormat(){
    int rowCount = tblTransactions.rowCount();
    int columnCount = tblTransactions.columnCount();
    if(columnCount < core::TransactionColumns::COLUMN_COUNT-2){
        core::Utils::showErrorMessage("Transaction table missing columns");
        return false;
    }
    bool ret = true;
    for(int row=0; row < rowCount; row++){
        int col = checkRowFormat(row);
        if(col != -1){
            tblTransactions.item(row, col)->setBackground(Qt::red);
            ret = false;
        }
    }
    return ret;
}

void Transactions::updateAllCellsFormat(){
    int rowCount = tblTransactions.rowCount();
    int columnCount = tblTransactions.columnCount();
    if(columnCount < core::TransactionColumns::COLUMN_COUNT-2){
        core::Utils::showErrorMessage("Transaction table missing columns");
        return;
    }
    for(int row=0; row < rowCount; row++){
        calculateAndSetColorOfRow(row, true);
    }
}

int Transactions::checkRowFormat(int row){
    std::string date = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->text().toStdString();
    std::string transactionAmount = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->text().toStdString();
    std::string balance = tblTransactions.item(row, core::TransactionColumns::BALANCE)->text().toStdString();
    if(!core::Utils::isDate(date))
        return core::TransactionColumns::TRANSACTION_DATE;
    if(!core::Utils::isNum(transactionAmount))
        return core::TransactionColumns::TRANSACTION_AMOUNT;
    if(!core::Utils::isNum(balance))
        return core::TransactionColumns::BALANCE;
    return -1;
}

bool Transactions::saveTransactionsToCore(){
    int rowCount = tblTransactions.rowCount();
    int columnCount = tblTransactions.columnCount();
    if(columnCount < core::TransactionColumns::COLUMN_COUNT-2){
        core::Utils::showErrorMessage("Transaction table missing columns");
        return false;
    }
    std::vector<core::Transaction> transactions;
    for(int row=0; row < rowCount; row++)
        transactions.push_back(rowToTransaction(row));
    _core->updateTransactions(transactions);
    updateAllCellsFormat();
    return true;
}

core::Transaction Transactions::rowToTransaction(int row){
    core::Transaction t;
    QTableWidgetItem* idItemPtr = tblTransactions.item(row, core::TransactionColumns::ID);
    if(idItemPtr == nullptr)
        return t;
    std::string id = tblTransactions.item(row, core::TransactionColumns::ID)->text().toStdString();
    if(!core::Utils::isNum(id) || id == "0")
        return t;
    std::string date = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->text().toStdString();
    std::string transactionAmount = tblTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->text().toStdString();
    std::string balance = tblTransactions.item(row, core::TransactionColumns::BALANCE)->text().toStdString();
    std::string description = tblTransactions.item(row, core::TransactionColumns::DESCRIPTION)->text().toStdString();
    std::string accountName = tblTransactions.item(row, core::TransactionColumns::ACCOUNT)->text().toStdString();
    t.setTransactionDate(core::Utils::toDate(date));
    t.setTransactionAmount(core::Utils::toInt(transactionAmount));
    t.setBalance(core::Utils::toInt(balance));
    t.setDescription(description);
    t.setFromAccount(accountName);
    t.setId(core::Utils::toInt(id));
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
    if(col){} // Remove warning
    calculateAndSetColorOfRow(row);
}

void Transactions::calculateAndSetColorOfRow(int row, bool alwaysSetColor){
    // Check that status colum has value (otherwise row is under construction)
    QTableWidgetItem* statusItemPtr = tblTransactions.item(row, core::TransactionColumns::STATUS);
    if(statusItemPtr == nullptr)
        return;

    // Compare status from column with format check of row. If it has turned to error change status column and color invalid column
    core::TransactionState stateFromTable = static_cast<core::TransactionState>(core::Utils::toInt(statusItemPtr->text().toStdString()));
    int errorCol = checkRowFormat(row);
    if(errorCol != -1){
        if(stateFromTable != core::TransactionState::ERROR){
            statusItemPtr->setText(std::to_string(core::TransactionState::ERROR).c_str());
            tblTransactions.item(row, errorCol)->setBackground(Qt::red);
        } else if(alwaysSetColor)
            tblTransactions.item(row, errorCol)->setBackground(Qt::red);
        return;
    }

    // Convert row to transaction, check its status and if it differs from status column, change color of row
    core::Transaction transaction = rowToTransaction(row);
    core::TransactionState stateFromCore = _core->getTransactionState(transaction);
    if(stateFromTable != stateFromCore){
        statusItemPtr->setText(std::to_string(stateFromCore).c_str());
        changeColorOfRow(row, stateFromCore);
    } else if(alwaysSetColor)
        changeColorOfRow(row, stateFromCore);
}

void Transactions::changeColorOfRow(int row, core::TransactionState state){
    QBrush color = Qt::transparent;
    if(state == core::TransactionState::UNCHANGED)
        color = Qt::transparent;
    else if(state == core::TransactionState::CHANGED)
        color = Qt::yellow;
    else if(state == core::TransactionState::NEW)
        color = Qt::green;
    else if(state == core::TransactionState::ERROR)
        color = Qt::red;
    for(int col=0; col<core::TransactionColumns::COLUMN_COUNT; col++)
        if(col != core::TransactionColumns::ID && col != core::TransactionColumns::STATUS)
            tblTransactions.item(row, col)->setBackground(color);
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

    if(checkAllCellsFormat()){
        saveTransactionsToCore();
    }
}

}
