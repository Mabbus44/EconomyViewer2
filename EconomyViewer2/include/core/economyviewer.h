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
    std::vector<std::shared_ptr<Transaction>> transactions;
    QScopedPointer<views::Transactions> transactionsView;
    QScopedPointer<views::NewTransactions> newTransactionsView;
    QScopedPointer<QFrame> window;

public slots:
    void openNewView(ViewNames viewName);
};

}
#endif // ECONOMYVIEWER_H
