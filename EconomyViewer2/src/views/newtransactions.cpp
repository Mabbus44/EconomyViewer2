#include "../../include/views/newtransactions.h"
#include "../../include/core/transaction.h"
#include "../../include/core/importedrow.h"
#include <fstream>
#include <QFileDialog>
#include <QSizePolicy>
#include <QtWidgets>

namespace views{

NewTransactions::NewTransactions() {
    createViewElements();
}

NewTransactions::NewTransactions(QWidget* parent):QFrame(parent){
    createViewElements();
}

void NewTransactions::createViewElements(){
    btnSaveTransactions.setParent(this);
    btnSaveTransactions.setText("Save transactions");
    btnSaveTransactions.setToolTip("Saves the loaded transactions to the main transaction pool");
    btnSaveTransactions.show();
    tblNewTransactions.setParent(this);
    tblNewTransactions.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    tblNewTransactions.setFixedHeight(300);
    tblNewTransactions.setFixedWidth(500);
    tblNewTransactions.show();
    cmbAccounts.setParent(this);
    cmbAccounts.show();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(&tblNewTransactions);
    layout->addWidget(&cmbAccounts);
    layout->addWidget(&btnSaveTransactions);
    this->setLayout(layout);

    this->show();
    QObject::connect(&btnSaveTransactions, &QPushButton::clicked, this, &NewTransactions::btnSaveTransactionsClick);
}

bool NewTransactions::openView(std::vector<core::Account>& accounts, bool loadFileDialog){
    bool ret = true;
    if(loadFileDialog)
        ret = loadTransactions();
    if(ret){
        loadAccounts(accounts);
        this->show();
    }
    return ret;
}

bool NewTransactions::openView(bool loadFileDialog){
    bool ret = true;
    if(loadFileDialog)
        ret = loadTransactions();
    if(ret)
        this->show();
    return ret;
}

bool NewTransactions::checkCellFormat(){
    int rowCount = tblNewTransactions.rowCount();
    int columnCount = tblNewTransactions.columnCount();
    if(columnCount < core::TransactionColumns::COLUMN_COUNT-1){
        core::Utils::showErrorMessage("Transaction table missing columns");
        return false;
    }
    bool ret = true;
    for(int row=0; row < rowCount; row++){
        std::string date = tblNewTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->text().toStdString();
        std::string transactionAmount = tblNewTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->text().toStdString();
        std::string balance = tblNewTransactions.item(row, core::TransactionColumns::BALANCE)->text().toStdString();
        std::string description = tblNewTransactions.item(row, core::TransactionColumns::DESCRIPTION)->text().toStdString();
        if(core::Utils::isDate(date))
            tblNewTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->setBackground(Qt::white);
        else{
            tblNewTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->setBackground(Qt::red);
            ret = false;
        }
        if(core::Utils::isNum(transactionAmount))
            tblNewTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->setBackground(Qt::white);
        else{
            tblNewTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->setBackground(Qt::red);
            ret = false;
        }
        if(core::Utils::isNum(balance))
            tblNewTransactions.item(row, core::TransactionColumns::BALANCE)->setBackground(Qt::white);
        else{
            tblNewTransactions.item(row, core::TransactionColumns::BALANCE)->setBackground(Qt::red);
            ret = false;
        }
    }
    return ret;
}

void NewTransactions::btnSaveTransactionsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    if(checkCellFormat()){
        std::string accountName = cmbAccounts.currentText().toStdString();
        std::vector<core::Transaction> transactions;
        int rowCount = tblNewTransactions.rowCount();
        int columnCount = tblNewTransactions.columnCount();
        if(columnCount < core::TransactionColumns::COLUMN_COUNT-1){
            core::Utils::showErrorMessage("Transaction table missing columns");
            return;
        }
        for(int row=0; row < rowCount; row++){
            std::string date = tblNewTransactions.item(row, core::TransactionColumns::TRANSACTION_DATE)->text().toStdString();
            std::string transactionAmount = tblNewTransactions.item(row, core::TransactionColumns::TRANSACTION_AMOUNT)->text().toStdString();
            std::string balance = tblNewTransactions.item(row, core::TransactionColumns::BALANCE)->text().toStdString();
            std::string description = tblNewTransactions.item(row, core::TransactionColumns::DESCRIPTION)->text().toStdString();
            if(core::Utils::isDate(date) && core::Utils::isNum(transactionAmount) && core::Utils::isNum(balance)){
                core::Transaction t;
                t.setTransactionDate(core::Utils::toDate(date));
                t.setTransactionAmount(core::Utils::toNum(transactionAmount));
                t.setBalance(core::Utils::toNum(balance));
                t.setDescription(description);
                t.setFromAccount(accountName);
                transactions.push_back(t);
            }
        }
        emit addNewTransactions(transactions);
        emit openTransactionsView(true);
    }
}

bool NewTransactions::loadTransactions(){
    // File dialog
    std::string fileName = QFileDialog::getOpenFileName(this, tr("Open account history"),"", tr("List files (*.csv)")).toStdString();

    //Read file
    std::ifstream file(fileName);
    if(!file.is_open())
        return false;
    std::string line;
    std::vector<core::ImportedRow> importedRows;
    while(std::getline(file, line)){
        core::ImportedRow row(line);
        importedRows.push_back(row);
    }

    // Count columns for each type
    std::map<int, int> dateColumns;
    std::map<int, int> textColumns;
    std::map<int, int> numColumns;
    for(core::ImportedRow& row: importedRows){
        if(row.isValid()){
            std::vector<int> rowDateColumns = row.dateColumns();
            std::vector<int> rowTextColumns = row.textColumns();
            std::vector<int> rowNumColumns = row.numColumns();
            for(int column: rowDateColumns){
                if (dateColumns.find(column) == dateColumns.end()) {
                    dateColumns[column] = 1;
                }else{
                    dateColumns[column] = dateColumns[column]+1;
                }
            }
            for(int column: rowTextColumns){
                if (textColumns.find(column) == textColumns.end()) {
                    textColumns[column] = 1;
                }else{
                    textColumns[column] = textColumns[column]+1;
                }
            }
            for(int column: rowNumColumns){
                if (numColumns.find(column) == numColumns.end()) {
                    numColumns[column] = 1;
                }else{
                    numColumns[column] = numColumns[column]+1;
                }
            }
        }
    }

    // Decide which date column to use if multiple
    int dateColumn = 0;
    int dateColumnsCount = 0;
    for (std::map<int, int>::iterator itr = dateColumns.begin(); itr != dateColumns.end(); ++itr) {
        dateColumnsCount++;
    }
    if(dateColumnsCount == 0){
        core::Utils::showErrorMessage("Could not find transaction dates in that file");
        return false;
    }
    if(dateColumnsCount == 1){
        dateColumn = dateColumns.begin()->first;
    }
    if(dateColumnsCount > 1){
        std::vector<std::tuple<int, int>> sortedDateColumns = core::Utils::sortKeysByVal(dateColumns);
        if(std::get<1>(sortedDateColumns[0]) > std::get<1>(sortedDateColumns[1])){
            dateColumn = std::get<0>(sortedDateColumns[0]);
        }else{  // If the best two date columns has the same number of rows, take the on with the oldest dates
            std::vector<std::tuple<int, int>> bestDateColumns = sortedDateColumns;
            while(std::get<1>(bestDateColumns[0]) > std::get<1>(bestDateColumns.back()))
                bestDateColumns.pop_back();
            bool correctColumnDetermined = false;
            for(core::ImportedRow& row: importedRows){
                bool allExist = true;
                bool first = true;
                bool uniqueOldestDate = true;
                std::time_t earliestDate;
                std::vector<int> rowDateColumns = row.dateColumns();
                for (std::vector<std::tuple<int, int>>::iterator itr = bestDateColumns.begin(); itr != bestDateColumns.end(); ++itr) {
                    // Check if column exist in current row
                    if(rowDateColumns.size() == 0 || std::find(rowDateColumns.begin(), rowDateColumns.end(),std::get<0>(*itr)) == rowDateColumns.end()){
                        allExist = false;
                    }else{
                        std::tm temp = core::Utils::toDate(row.columns()[std::get<0>(*itr)]);
                        std::time_t columnDate = std::mktime(&temp);
                        if(first){
                            earliestDate = columnDate;
                            dateColumn = std::get<0>(*itr);
                            uniqueOldestDate = true;
                            first = false;
                        }else{
                            if(columnDate < earliestDate){
                                earliestDate = columnDate;
                                dateColumn = std::get<0>(*itr);
                                uniqueOldestDate = true;
                            }else if(columnDate == earliestDate){
                                uniqueOldestDate = false;
                            }
                        }
                    }
                }
                if(allExist && uniqueOldestDate){
                    correctColumnDetermined = true;
                    break;
                }
            }
            if(!correctColumnDetermined){
                core::Utils::showErrorMessage("Could not determine correct transaction dates column in that file");
                return false;
            }
        }
    }

    // Decide which text column to use if multiple
    int textColumn = 0;
    int textColumnsCount = 0;
    for (std::map<int, int>::iterator itr = textColumns.begin(); itr != textColumns.end(); ++itr) {
        textColumnsCount++;
    }
    if(textColumnsCount == 0){
        core::Utils::showErrorMessage("Could not find descriptions in that file");
        return false;
    }
    if(textColumnsCount == 1){
        textColumn = textColumns.begin()->first;
    }
    if(textColumnsCount > 1){
        textColumn = std::get<0>(core::Utils::sortKeysByVal(textColumns)[0]);
    }

    // Decide which balance columns to use if multiple
    int balanceColumn = 0;
    int transactionColumn = 0;
    int numColumnsCount = 0;
    int bestPositiveRowCount = -1;
    int bestNegativeRowCount = -1;
    for (std::map<int, int>::iterator itr = numColumns.begin(); itr != numColumns.end(); ++itr) {
        int columnNr = std::get<0>(*itr);
        int positiveRowCount = 0;
        int negativeRowCount = 0;
        for(core::ImportedRow& row: importedRows){
            if(row.isColumnNum(columnNr) && row.getColumnAsNum(columnNr) >= 0){
                positiveRowCount++;
            }
            if(row.isColumnNum(columnNr) && row.getColumnAsNum(columnNr) < 0){
                negativeRowCount++;
            }
        }
        if(positiveRowCount > bestPositiveRowCount){
            bestPositiveRowCount = positiveRowCount;
            balanceColumn = columnNr;
        }
        if(negativeRowCount > bestNegativeRowCount){
            bestNegativeRowCount = negativeRowCount;
            transactionColumn = columnNr;
        }
        numColumnsCount++;
    }
    if(numColumnsCount <= 1){
        core::Utils::showErrorMessage("Could not find balance or transaction amounts in that file");
        return false;
    }

    // Convert rows to transactions
    std::vector<core::Transaction> newTransactions;
    for(core::ImportedRow& row: importedRows){
        core::Transaction newTransaction;
        bool validRow = true;
        if(row.isColumnDate(dateColumn))
            newTransaction.setTransactionDate(row.getColumnAsDate(dateColumn));
        else
            validRow = false;
        if(row.isColumnNum(transactionColumn))
            newTransaction.setTransactionAmount(row.getColumnAsNum(transactionColumn));
        else
            validRow = false;
        if(row.isColumnNum(balanceColumn))
            newTransaction.setBalance(row.getColumnAsNum(balanceColumn));
        else
            validRow = false;
        if(row.isColumnText(textColumn))
            newTransaction.setDescription(row.getColumnAsText(textColumn));
        if(validRow){
            newTransactions.push_back(newTransaction);
        }
    }

    // Populate the table
    tblNewTransactions.setRowCount(newTransactions.size());
    tblNewTransactions.setColumnCount(core::TransactionColumns::COLUMN_COUNT-1);
    tblNewTransactions.setHorizontalHeaderLabels({"Date","Transfer amount","Balance","Comment"});
    int row = 0;
    for(core::Transaction& transaction: newTransactions){
        tblNewTransactions.setItem(row, core::TransactionColumns::TRANSACTION_DATE, new QTableWidgetItem(transaction.getTransactionDateAsString().c_str()));
        tblNewTransactions.setItem(row, core::TransactionColumns::TRANSACTION_AMOUNT, new QTableWidgetItem(transaction.getTransactionAmountAsString().c_str()));
        tblNewTransactions.setItem(row, core::TransactionColumns::BALANCE, new QTableWidgetItem(transaction.getBalanceAsString().c_str()));
        tblNewTransactions.setItem(row, core::TransactionColumns::DESCRIPTION, new QTableWidgetItem(transaction.getDescriptionAsString().c_str()));
        row++;
    }
    tblNewTransactions.resizeColumnsToContents();
    checkCellFormat();
    return true;
}

bool NewTransactions::loadAccounts(std::vector<core::Account>& accounts){
    cmbAccounts.clear();
    for(core::Account account: accounts)
        cmbAccounts.addItem(account.name().c_str());
    return true;
}
}
