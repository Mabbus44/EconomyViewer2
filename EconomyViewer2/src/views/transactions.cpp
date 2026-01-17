#include "../../include/core/economyviewer.h"
#include "../../include/views/transactions.h"
#include "../../include/core/constants.h"
#include <QMessageBox>
#include <QSizePolicy>
#include <QColor>
#include <QLayout>
#include <QFileDialog>
#include <QHeaderView>

namespace views{

Transactions::Transactions() {
    createViewElements();
}

Transactions::Transactions(QWidget* parent, core::EconomyViewer* core):QFrame(parent){
    _core = core;
    createViewElements();
}

bool Transactions::openView(){
    _core->layout->setCurrentWidget(this);
    return true;
}

bool Transactions::openView(std::map<unsigned int, core::Transaction>& transactions, bool appendTransactions){
    std::vector<core::Transaction> transVector;
    for(auto const& p: transactions)
        transVector.push_back(p.second);
    return openView(transVector, appendTransactions);
}

bool Transactions::openView(std::vector<core::Transaction>& transactions, bool appendTransactions){
    _core->layout->setCurrentWidget(this);
    int row = 0;
    if(!appendTransactions || tblTransactions.rowCount() <= 0){
        tblTransactions.setRowCount(transactions.size());
        tblTransactions.setColumnCount(TransactionColumns::COLUMN_COUNT);
        tblTransactions.setHorizontalHeaderLabels({"Date","Transfer amount","Balance","Comment","Account", "Group", "Id", "Status"});
        tblTransactions.hideColumn(TransactionColumns::ID);
        tblTransactions.hideColumn(TransactionColumns::STATUS);
    }else{
        row = tblTransactions.rowCount();
        tblTransactions.setRowCount(tblTransactions.rowCount() + transactions.size());
    }
    for(core::Transaction& transaction: transactions){
        tblTransactions.setItem(row, TransactionColumns::TRANSACTION_DATE, new QTableWidgetItem(transaction.getTransactionDateAsString().c_str()));
        tblTransactions.setItem(row, TransactionColumns::TRANSACTION_AMOUNT, new QTableWidgetItem(transaction.getTransactionAmountAsString().c_str()));
        tblTransactions.setItem(row, TransactionColumns::BALANCE, new QTableWidgetItem(transaction.getBalanceAsString().c_str()));
        tblTransactions.setItem(row, TransactionColumns::DESCRIPTION, new QTableWidgetItem(transaction.description().c_str()));
        tblTransactions.setItem(row, TransactionColumns::ACCOUNT, new QTableWidgetItem(transaction.fromAccount().c_str()));
        tblTransactions.setItem(row, TransactionColumns::GROUP, new QTableWidgetItem(transaction.group().c_str()));
        tblTransactions.setItem(row, TransactionColumns::ID, new QTableWidgetItem(transaction.getIdAsString().c_str()));
        TableRowState::TableRowState state;
        int errorCol = checkRowFormat(row);
        if(errorCol == -1)
            state = _core->getTransactionTableRowState(transaction);
        else
            state = TableRowState::ERROR;
        changeColorOfRow(row, state);
        tblTransactions.setItem(row, TransactionColumns::STATUS, new QTableWidgetItem(std::to_string(state).c_str()));
        row++;
    }
    tblTransactions.resizeColumnsToContents();
    tblTransactions.horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tblTransactions.horizontalHeader()->setStretchLastSection(true);
    return true;
}

bool Transactions::checkAllCellsFormat(){
    int rowCount = tblTransactions.rowCount();
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
    for(int row=0; row < rowCount; row++){
        calculateAndSetColorOfRow(row, true);
    }
}

int Transactions::checkRowFormat(int row){
    std::string date = tblTransactions.item(row, TransactionColumns::TRANSACTION_DATE)->text().toStdString();
    std::string transactionAmount = tblTransactions.item(row, TransactionColumns::TRANSACTION_AMOUNT)->text().toStdString();
    std::string balance = tblTransactions.item(row, TransactionColumns::BALANCE)->text().toStdString();
    if(!core::Utils::isDate(date))
        return TransactionColumns::TRANSACTION_DATE;
    if(!core::Utils::isNum(transactionAmount))
        return TransactionColumns::TRANSACTION_AMOUNT;
    if(!core::Utils::isNum(balance))
        return TransactionColumns::BALANCE;
    return -1;
}

bool Transactions::saveTransactionsToCore(){
    int rowCount = tblTransactions.rowCount();
    std::vector<core::Transaction> transactions;
    for(int row=0; row < rowCount; row++)
        transactions.push_back(rowToTransaction(row));
    _core->updateTransactions(transactions);
    updateAllCellsFormat();
    return true;
}

core::Transaction Transactions::rowToTransaction(int row){
    core::Transaction t;
    QTableWidgetItem* idItemPtr = tblTransactions.item(row, TransactionColumns::ID);
    if(idItemPtr == nullptr)
        return t;
    std::string id = tblTransactions.item(row, TransactionColumns::ID)->text().toStdString();
    if(!core::Utils::isNum(id) || id == "0")
        return t;
    std::string date = tblTransactions.item(row, TransactionColumns::TRANSACTION_DATE)->text().toStdString();
    std::string transactionAmount = tblTransactions.item(row, TransactionColumns::TRANSACTION_AMOUNT)->text().toStdString();
    std::string balance = tblTransactions.item(row, TransactionColumns::BALANCE)->text().toStdString();
    std::string description = tblTransactions.item(row, TransactionColumns::DESCRIPTION)->text().toStdString();
    std::string accountName = tblTransactions.item(row, TransactionColumns::ACCOUNT)->text().toStdString();
    std::string group = tblTransactions.item(row, TransactionColumns::GROUP)->text().toStdString();
    t.setTransactionDate(core::Utils::toDate(date));
    t.setTransactionAmount(core::Utils::toInt(transactionAmount));
    t.setBalance(core::Utils::toInt(balance));
    t.setDescription(description);
    t.setFromAccount(accountName);
    t.setGroup(group);
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

    btnManageTransactionGroups.setParent(this);
    btnManageTransactionGroups.setText("Manage transaction groups");
    btnManageTransactionGroups.setToolTip("Add, remove or edit transaction groups");
    btnManageTransactionGroups.show();

    btnApplyTransactionGroups.setParent(this);
    btnApplyTransactionGroups.setText("Apply transaction groups");
    btnApplyTransactionGroups.setToolTip("Applies all transaction group rules to all transaction without a group");
    btnApplyTransactionGroups.show();

    btnApplyChanges.setParent(this);
    btnApplyChanges.setText("Apply changes");
    btnApplyChanges.setToolTip("Applies all changes of transactions (does not save to file)");
    btnApplyChanges.show();

    btnSaveToFile.setParent(this);
    btnSaveToFile.setText("Save to file");
    btnSaveToFile.setToolTip("Saves transactions, accounts and transactionGroups to file");
    btnSaveToFile.show();

    btnLoadFromFile.setParent(this);
    btnLoadFromFile.setText("Load from file");
    btnLoadFromFile.setToolTip("Loads transactions, accounts and transactionGroups from file");
    btnLoadFromFile.show();

    btnViewGraph.setParent(this);
    btnViewGraph.setText("View graph");
    btnViewGraph.setToolTip("View graph of accumulated transactions over time");
    btnViewGraph.show();

    tblTransactions.setParent(this);
    tblTransactions.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tblTransactions.show();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(&tblTransactions);
    layout->addWidget(&btnAddTransactions);
    layout->addWidget(&btnManageAccounts);
    layout->addWidget(&btnManageTransactionGroups);
    layout->addWidget(&btnApplyTransactionGroups);
    layout->addWidget(&btnApplyChanges);
    layout->addWidget(&btnSaveToFile);
    layout->addWidget(&btnLoadFromFile);
    layout->addWidget(&btnViewGraph);
    this->setLayout(layout);

    this->show();
    QObject::connect(&tblTransactions, &QTableWidget::cellChanged, this, &Transactions::tblTransactionsChanged);
    QObject::connect(&btnAddTransactions, &QPushButton::clicked, this, &Transactions::btnAddTransactionsClick);
    QObject::connect(&btnManageAccounts, &QPushButton::clicked, this, &Transactions::btnManageAccountsClick);
    QObject::connect(&btnManageTransactionGroups, &QPushButton::clicked, this, &Transactions::btnManageTransactionGroupsClick);
    QObject::connect(&btnApplyTransactionGroups, &QPushButton::clicked, this, &Transactions::btnApplyTransactionGroupsClick);
    QObject::connect(&btnApplyChanges, &QPushButton::clicked, this, &Transactions::btnApplyChangesClick);
    QObject::connect(&btnSaveToFile, &QPushButton::clicked, this, &Transactions::btnSaveToFileClick);
    QObject::connect(&btnLoadFromFile, &QPushButton::clicked, this, &Transactions::btnLoadFromFileClick);
    QObject::connect(&btnViewGraph, &QPushButton::clicked, this, &Transactions::btnViewGraphClick);
}

void Transactions::tblTransactionsChanged(int row, int col){
    if(col){} // Remove warning
    calculateAndSetColorOfRow(row);
}

void Transactions::calculateAndSetColorOfRow(int row, bool alwaysSetColor){
    // Check that status colum has value (otherwise row is under construction)
    QTableWidgetItem* statusItemPtr = tblTransactions.item(row, TransactionColumns::STATUS);
    if(statusItemPtr == nullptr)
        return;

    // Compare status from column with format check of row. If it has turned to error change status column and color invalid column
    TableRowState::TableRowState stateFromTable = static_cast<TableRowState::TableRowState>(core::Utils::toInt(statusItemPtr->text().toStdString()));
    int errorCol = checkRowFormat(row);
    if(errorCol != -1){
        if(stateFromTable != TableRowState::ERROR){
            statusItemPtr->setText(std::to_string(TableRowState::ERROR).c_str());
            tblTransactions.item(row, errorCol)->setBackground(Qt::red);
        } else if(alwaysSetColor)
            tblTransactions.item(row, errorCol)->setBackground(Qt::red);
        return;
    }

    // Convert row to transaction, check its status and if it differs from status column, change color of row
    core::Transaction transaction = rowToTransaction(row);
    TableRowState::TableRowState stateFromCore = _core->getTransactionTableRowState(transaction);
    if(stateFromTable != stateFromCore){
        statusItemPtr->setText(std::to_string(stateFromCore).c_str());
        changeColorOfRow(row, stateFromCore);
    } else if(alwaysSetColor)
        changeColorOfRow(row, stateFromCore);
}

void Transactions::changeColorOfRow(int row, TableRowState::TableRowState state){
    QBrush color = Qt::transparent;
    if(state == TableRowState::UNCHANGED)
        color = Qt::transparent;
    else if(state == TableRowState::CHANGED)
        color = Qt::yellow;
    else if(state == TableRowState::NEW)
        color = Qt::green;
    else if(state == TableRowState::ERROR)
        color = Qt::red;
    for(int col=0; col<TransactionColumns::COLUMN_COUNT; col++)
        if(col != TransactionColumns::ID && col != TransactionColumns::STATUS)
            tblTransactions.item(row, col)->setBackground(color);
}

void Transactions::applyTransactionGroups(){
    int rowCount = tblTransactions.rowCount();
    for(int row=0; row < rowCount; row++){
        core::Transaction transaction = rowToTransaction(row);
        if(core::Utils::isEmpty(transaction.group())){
            for(auto& [key, val]: _core->transactionGroupsRef()){
                if(val.isMatch(transaction)){
                    tblTransactions.setItem(row, TransactionColumns::GROUP, new QTableWidgetItem(val.groupName().c_str()));
                    break;
                }
            }
        }
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

void Transactions::btnManageTransactionGroupsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    _core->openTransactionGroupsView(true);
}

void Transactions::btnApplyTransactionGroupsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    applyTransactionGroups();
}

void Transactions::btnApplyChangesClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    if(checkAllCellsFormat()){
        saveTransactionsToCore();
    }
}

void Transactions::btnSaveToFileClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    std::string fileName = QFileDialog::getSaveFileName(this, tr("Save to file"),"", tr("List files (*.ecv)")).toStdString();
    if(!_core->saveToFile(fileName))
        core::Utils::showErrorMessage("Could not save file");
}

void Transactions::btnLoadFromFileClick(bool checked){
    if(checked){}     //Remove warning "checked unused"

    std::string fileName = QFileDialog::getOpenFileName(this, tr("Load from file"),"", tr("List files (*.ecv)")).toStdString();
    if(!_core->loadFromFile(fileName))
        core::Utils::showErrorMessage("Could not load file");
    else
        _core->openTransactionsView(true);
}

void Transactions::btnViewGraphClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    _core->openGraphView();
}

}
