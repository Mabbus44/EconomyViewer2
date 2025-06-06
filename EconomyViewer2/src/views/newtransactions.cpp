#include "../../include/views/newtransactions.h"

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
    this->show();
    QObject::connect(&btnSaveTransactions, &QPushButton::clicked, this, &NewTransactions::btnSaveTransactionsClick);
}

void NewTransactions::btnSaveTransactionsClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    emit changedView(core::ViewNames::TRANSACTIONS);
}

}
