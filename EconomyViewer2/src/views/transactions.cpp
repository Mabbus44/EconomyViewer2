#include "../../include/views/transactions.h"
#include "../../include/core/constants.h"
#include <QMessageBox>

namespace views{

Transactions::Transactions() {
    createViewElements();
}

Transactions::Transactions(QWidget* parent):QFrame(parent){
    createViewElements();
}

void Transactions::createViewElements(){
    btnAddTransactions.setParent(this);
    btnAddTransactions.setText("Add new transactions");
    btnAddTransactions.setToolTip("Loads new transactions from a file (usually an export from an internet banks account history)");
    btnAddTransactions.show();
    this->show();
    QObject::connect(&btnAddTransactions, &QPushButton::clicked, this, &Transactions::btnAddTransactionsClick);
}

void Transactions::btnAddTransactionsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    emit changedView(core::ViewNames::NEW_TRANSACTIONS);
}

}
