#include "../../include/core/economyviewer.h"
#include "../../include/views/transactionGroups.h"
#include <QSizePolicy>
#include <QLayout>

namespace views{

TransactionGroups::TransactionGroups() {
    createViewElements();
}

TransactionGroups::TransactionGroups(QWidget* parent, core::EconomyViewer* core):QFrame(parent){
    _core = core;
    createViewElements();
}

void TransactionGroups::createViewElements(){
    btnAddNewGroup.setParent(this);
    btnAddNewGroup.setText("Add new group");
    btnAddNewGroup.setToolTip("Adds new transaction group to the list");
    btnAddNewGroup.show();
    btnDeleteGroup.setParent(this);
    btnDeleteGroup.setText("Delete group");
    btnDeleteGroup.setToolTip("Deletes selection transaction group");
    btnDeleteGroup.show();
    btnEditGroup.setParent(this);
    btnEditGroup.setText("Edit group");
    btnEditGroup.setToolTip("Edit selected transation group");
    btnEditGroup.show();
    btnSaveChanges.setParent(this);
    btnSaveChanges.setText("Save changes");
    btnSaveChanges.setToolTip("Save changes to the list and return to main page");
    btnSaveChanges.show();

    tblTransactionGroups.setParent(this);
    tblTransactionGroups.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    tblTransactionGroups.setFixedHeight(300);
    tblTransactionGroups.setFixedWidth(500);
    tblTransactionGroups.show();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(&tblTransactionGroups);
    layout->addWidget(&btnAddNewGroup);
    layout->addWidget(&btnDeleteGroup);
    layout->addWidget(&btnEditGroup);
    layout->addWidget(&btnSaveChanges);
    this->setLayout(layout);

    this->show();
    QObject::connect(&btnAddNewGroup, &QPushButton::clicked, this, &TransactionGroups::btnAddNewGroupClick);
    QObject::connect(&btnDeleteGroup, &QPushButton::clicked, this, &TransactionGroups::btnDeleteGroupClick);
    QObject::connect(&btnEditGroup, &QPushButton::clicked, this, &TransactionGroups::btnEditGroupClick);
    QObject::connect(&btnSaveChanges, &QPushButton::clicked, this, &TransactionGroups::btnSaveChangesClick);
}

bool TransactionGroups::openView(){
    std::map<unsigned int, core::TransactionGroup> emptyMap;
    return openView(emptyMap, true);
}

bool TransactionGroups::openView(std::map<unsigned int, core::TransactionGroup>& transactionGroups, bool appendTransactionGroups){
    if(appendTransactionGroups){
        for(auto& mapEntry: transactionGroups)
            _transactionGroups[mapEntry.first] = mapEntry.second;
    }else
        _transactionGroups = transactionGroups;

    tblTransactionGroups.clear();
    tblTransactionGroups.setRowCount(_transactionGroups.size());
    tblTransactionGroups.setColumnCount(TransactionGroupColumns::COLUMN_COUNT);
    tblTransactionGroups.hideColumn(TransactionGroupColumns::ID);
    tblTransactionGroups.setHorizontalHeaderLabels({"Account name","Rules"});

    int row = 0;
    for(auto& mapEntry: _transactionGroups){
        tblTransactionGroups.setItem(row, TransactionGroupColumns::ACCOUNT_NAME, new QTableWidgetItem(mapEntry.second.accountName().c_str()));
        tblTransactionGroups.setItem(row, TransactionGroupColumns::RULES, new QTableWidgetItem(std::to_string(mapEntry.second.conditionCount()).c_str()));
        tblTransactionGroups.setItem(row, TransactionGroupColumns::ID, new QTableWidgetItem(std::to_string(mapEntry.second.id()).c_str()));
        row++;
    }
    tblTransactionGroups.resizeColumnsToContents();
    this->show();
    return true;
}

void TransactionGroups::btnAddNewGroupClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    int rowCount = tblTransactionGroups.rowCount();
    tblTransactionGroups.setRowCount(rowCount+1);
    core::TransactionGroup newGroup;
    newGroup.accountName("No account");
    newGroup.accountNumber("0");
    newGroup.id(_core->getUniqueTransactionGroupId());
    _transactionGroups[newGroup.id()] = newGroup;
    tblTransactionGroups.setItem(rowCount, TransactionGroupColumns::ACCOUNT_NAME, new QTableWidgetItem(newGroup.accountName().c_str()));
    tblTransactionGroups.setItem(rowCount, TransactionGroupColumns::RULES, new QTableWidgetItem(std::to_string(newGroup.conditionCount()).c_str()));
    tblTransactionGroups.setItem(rowCount, TransactionGroupColumns::ID, new QTableWidgetItem(std::to_string(newGroup.id()).c_str()));
}

void TransactionGroups::btnDeleteGroupClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    QList<QTableWidgetItem*> selectedItems = tblTransactionGroups.selectedItems();
    if(selectedItems.size() == 0)
        return;
    int row = selectedItems[0]->row();
    unsigned int id = core::Utils::toUInt(tblTransactionGroups.item(row, TransactionGroupColumns::ID)->text().toStdString());
    _transactionGroups.erase(id);
    tblTransactionGroups.removeRow(selectedItems[0]->row());
}

void TransactionGroups::btnSaveChangesClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    _core->updateTransactionGroups(_transactionGroups);
    _core->openTransactionsView();
}

void TransactionGroups::btnEditGroupClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    QList<QTableWidgetItem*> selectedItems = tblTransactionGroups.selectedItems();
    if(selectedItems.size() == 0)
        return;
    int row = selectedItems[0]->row();
    unsigned int id = core::Utils::toUInt(tblTransactionGroups.item(row, TransactionGroupColumns::ID)->text().toStdString());
    _core->openMatchConditionsView(_transactionGroups[id]);
}
}
