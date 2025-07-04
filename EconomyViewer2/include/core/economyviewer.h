#ifndef ECONOMYVIEWER_H
#define ECONOMYVIEWER_H

#include <QApplication>
#include <QFrame>
#include "../core/constants.h"
#include "../core/transaction.h"
#include "../views/transactions.h"
#include "../views/newtransactions.h"

namespace core{

class EconomyViewer: public QObject
{
Q_OBJECT
public:
    EconomyViewer();
    int run(int argc, char **argv);
    void closeCurrentView();

    ViewNames currentView = ViewNames::NONE;
    std::vector<Transaction> transactions;
    QScopedPointer<views::Transactions> transactionsView;
    QScopedPointer<views::NewTransactions> newTransactionsView;
    QScopedPointer<QFrame> window;
    void updateTransactionsTable();

public slots:
    void openNewView(ViewNames viewName);
    void addNewTransactions(std::vector<Transaction>& newTransactions);
};

}
#endif // ECONOMYVIEWER_H
