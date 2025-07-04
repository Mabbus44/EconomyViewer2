#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include "../core/constants.h"

namespace views{

class Transactions: public QFrame
{
Q_OBJECT
public:
    Transactions();
    Transactions(QWidget* parent);
    QPushButton btnAddTransactions;
    QTableWidget tblTransactions;
private:
    void createViewElements();

signals:
    void changedView(core::ViewNames newValue);
public slots:
    void btnAddTransactionsClick(bool checked);

};

}

#endif // TRANSACTIONS_H
