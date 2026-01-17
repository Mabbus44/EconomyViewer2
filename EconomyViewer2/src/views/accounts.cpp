#include "../../include/core/economyviewer.h"
#include "../../include/views/accounts.h"
#include <QFileDialog>
#include <QSizePolicy>
#include <QLayout>
#include <QHeaderView>

namespace views{

Accounts::Accounts() {
    createViewElements();
}

Accounts::Accounts(QWidget* parent, core::EconomyViewer* core):QFrame(parent){
    _core = core;
    createViewElements();
}

void Accounts::createViewElements(){
    btnAddNewAccount.setParent(this);
    btnAddNewAccount.setText("Add new account");
    btnAddNewAccount.setToolTip("Adds new account to the list");
    btnAddNewAccount.show();
    btnDeleteAccount.setParent(this);
    btnDeleteAccount.setText("Delete account");
    btnDeleteAccount.setToolTip("Deletes selected account");
    btnDeleteAccount.show();
    btnSaveChanges.setParent(this);
    btnSaveChanges.setText("Save changes");
    btnSaveChanges.setToolTip("Save changes to the accounts");
    btnSaveChanges.show();

    tblAccounts.setParent(this);
    tblAccounts.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    tblAccounts.show();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(&tblAccounts);
    layout->addWidget(&btnAddNewAccount);
    layout->addWidget(&btnDeleteAccount);
    layout->addWidget(&btnSaveChanges);
    this->setLayout(layout);

    this->show();
    QObject::connect(&btnAddNewAccount, &QPushButton::clicked, this, &Accounts::btnAddNewAccountClick);
    QObject::connect(&btnDeleteAccount, &QPushButton::clicked, this, &Accounts::btnDeleteAccountClick);
    QObject::connect(&btnSaveChanges, &QPushButton::clicked, this, &Accounts::btnSaveChangesClick);
}

bool Accounts::openView(){
    _core->layout->setCurrentWidget(this);
    return true;
}

bool Accounts::openView(std::vector<core::Account>& accounts){
    _core->layout->setCurrentWidget(this);
    tblAccounts.setRowCount(accounts.size());
    tblAccounts.setColumnCount(2);
    tblAccounts.setHorizontalHeaderLabels({"Name","Account number"});
    int row = 0;
    for(core::Account& account: accounts){
        tblAccounts.setItem(row, 0, new QTableWidgetItem(account.name().c_str()));
        tblAccounts.setItem(row, 1, new QTableWidgetItem(account.accountNumber().c_str()));
        row++;
    }
    tblAccounts.resizeColumnsToContents();
    tblAccounts.horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tblAccounts.horizontalHeader()->setStretchLastSection(true);
    return true;
}

void Accounts::btnAddNewAccountClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    int rowCount = tblAccounts.rowCount();
    tblAccounts.setRowCount(rowCount+1);
    tblAccounts.setItem(rowCount, 0, new QTableWidgetItem("New account"));
    tblAccounts.setItem(rowCount, 1, new QTableWidgetItem("123-456-789"));
}

void Accounts::btnDeleteAccountClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    QList<QTableWidgetItem*> selectedItems = tblAccounts.selectedItems();
    if(selectedItems.size() == 0)
        return;
    tblAccounts.removeRow(selectedItems[0]->row());
}

void Accounts::btnSaveChangesClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    if(checkCellFormat()){
        std::vector<core::Account> accounts;
        int rowCount = tblAccounts.rowCount();
        int columnCount = tblAccounts.columnCount();
        if(columnCount < 2){
            core::Utils::showErrorMessage("Accounts table missing columns");
            return;
        }
        for(int row=0; row < rowCount; row++){
            std::string name = tblAccounts.item(row, 0)->text().toStdString();
            std::string accountNumber = tblAccounts.item(row, 1)->text().toStdString();
            core::Account account;
            account.name(name);
            account.accountNumber(accountNumber);
            accounts.push_back(account);
        }
        _core->updateAccounts(accounts);
        _core->openTransactionsView(true);
    }
}

bool Accounts::checkCellFormat(){
    int rowCount = tblAccounts.rowCount();
    int columnCount = tblAccounts.columnCount();
    if(columnCount < 2){
        core::Utils::showErrorMessage("Accounts table missing columns");
        return false;
    }
    bool cellsOk = true;
    std::vector<std::string> usedAccountNumbers;
    for(int row=0; row < rowCount; row++){
        std::string name = tblAccounts.item(row, 0)->text().toStdString();
        std::string accountNumber = tblAccounts.item(row, 1)->text().toStdString();
        bool rowOk = true;
        for(std::string& usedAccountNumber: usedAccountNumbers)
            if(accountNumber == usedAccountNumber)
                rowOk = false;
        if(name == "" || accountNumber == "")
            rowOk = false;
        if(rowOk){
            tblAccounts.item(row, 0)->setBackground(Qt::white);
            tblAccounts.item(row, 1)->setBackground(Qt::white);
        }else{
            cellsOk = false;
            tblAccounts.item(row, 0)->setBackground(Qt::red);
            tblAccounts.item(row, 1)->setBackground(Qt::red);
        }
        usedAccountNumbers.push_back(accountNumber);
    }
    return cellsOk;
}
}
