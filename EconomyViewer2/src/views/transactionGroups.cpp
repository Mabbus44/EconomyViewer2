#include "../../include/core/economyviewer.h"
#include "../../include/views/transactionGroups.h"
#include <QSizePolicy>
#include <QtWidgets>

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
    tblTransactionGroups.setRowCount(3);
    tblTransactionGroups.setColumnCount(2);
    tblTransactionGroups.setHorizontalHeaderLabels({"Name","Hidden id"});
    tblTransactionGroups.setItem(0, 0, new QTableWidgetItem("First groups"));
    tblTransactionGroups.setItem(0, 1, new QTableWidgetItem("Grosserys"));
    tblTransactionGroups.setItem(0, 2, new QTableWidgetItem("Car"));

    this->show();
    return true;
}

void TransactionGroups::btnAddNewGroupClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    int rowCount = tblTransactionGroups.rowCount();
    tblTransactionGroups.setRowCount(rowCount+1);
    tblTransactionGroups.setItem(rowCount, 0, new QTableWidgetItem("New group"));
}

void TransactionGroups::btnDeleteGroupClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    QList<QTableWidgetItem*> selectedItems = tblTransactionGroups.selectedItems();
    if(selectedItems.size() == 0)
        return;
    tblTransactionGroups.removeRow(selectedItems[0]->row());
}

void TransactionGroups::btnSaveChangesClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    if(checkCellFormat()){
    }
}

void TransactionGroups::btnEditGroupClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
}

bool TransactionGroups::checkCellFormat(){
    int rowCount = tblTransactionGroups.rowCount();
    int columnCount = tblTransactionGroups.columnCount();
    if(columnCount < 1){
        core::Utils::showErrorMessage("Groups table missing columns");
        return false;
    }
    bool cellsOk = true;
    std::vector<std::string> usedGroupNames;
    for(int row=0; row < rowCount; row++){
        std::string name = tblTransactionGroups.item(row, 0)->text().toStdString();
        bool rowOk = true;
        for(std::string& usedGroupName: usedGroupNames)
            if(name == usedGroupName)
                rowOk = false;
        if(name == "")
            rowOk = false;
        if(rowOk){
            tblTransactionGroups.item(row, 0)->setBackground(Qt::white);
        }else{
            cellsOk = false;
            tblTransactionGroups.item(row, 0)->setBackground(Qt::red);
        }
        usedGroupNames.push_back(name);
    }
    return cellsOk;
}
}
