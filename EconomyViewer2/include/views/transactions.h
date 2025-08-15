#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include "../core/constants.h"
#include "../core/transaction.h"

namespace views{

class Transactions: public QFrame
{
Q_OBJECT
public:
    Transactions();
    Transactions(QWidget* parent);
    bool openView();
    bool openView(std::vector<core::Transaction>& transactions);
    QPushButton btnAddTransactions;
    QPushButton btnManageAccounts;
    QTableWidget tblTransactions;
private:
    void createViewElements();

signals:
    void openNewTransactionsView(bool openFileDialog = false);
    void openAccountsView(bool useCoreAccountsVector = false);
public slots:
    void btnAddTransactionsClick(bool checked);
    void btnManageAccountsClick(bool checked);

};

}

#endif // TRANSACTIONS_H
