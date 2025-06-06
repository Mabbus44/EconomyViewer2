#include "../../include/core/economyviewer.h"
#include "../../include/core/constants.h"
#include <QMessageBox>

namespace core{

EconomyViewer::EconomyViewer() {}

int EconomyViewer::run(int argc, char **argv) {
    QApplication app (argc, argv);
    window.reset(new QFrame());
    window->show();

    transactionsView.reset(new views::Transactions(window.get()));
    QObject::connect(transactionsView.get(), &views::Transactions::changedView, this, &EconomyViewer::openNewView);
    transactionsView->show();
    currentView = ViewNames::TRANSACTIONS;

    newTransactionsView.reset(new views::NewTransactions(window.get()));
    QObject::connect(newTransactionsView.get(), &views::NewTransactions::changedView, this, &EconomyViewer::openNewView);
    newTransactionsView->hide();

    return app.exec();
}

void EconomyViewer::closeCurrentView(){
    switch(currentView){
    case ViewNames::TRANSACTIONS:
        transactionsView->hide();
        break;
    case ViewNames::NEW_TRANSACTIONS:
        newTransactionsView->hide();
        break;
    case ViewNames::MANAGE_ACCOUNTS:
        break;
    case ViewNames::TRANSACTIONS_GROUPS:
        break;
    case ViewNames::EDIT_TRANSACTION_GROUP:
        break;
    case ViewNames::CHANGE_TRANSACTIONS:
        break;
    default:
        break;
    }
    currentView = ViewNames::NONE;
}

void EconomyViewer::openNewView(ViewNames viewName){
    if(!window){
        showErrorMessage("Cannot open view without a window");
        return;
    }

    closeCurrentView();
    switch(viewName){
    case ViewNames::TRANSACTIONS:
        transactionsView->show();
        currentView = ViewNames::TRANSACTIONS;
        break;
    case ViewNames::NEW_TRANSACTIONS:
        newTransactionsView->show();
        currentView = ViewNames::NEW_TRANSACTIONS;
        break;
    case ViewNames::MANAGE_ACCOUNTS:
        break;
    case ViewNames::TRANSACTIONS_GROUPS:
        break;
    case ViewNames::EDIT_TRANSACTION_GROUP:
        break;
    case ViewNames::CHANGE_TRANSACTIONS:
        break;
    default:
        break;
    }
}

}
