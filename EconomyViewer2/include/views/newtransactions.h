#ifndef NEWTRANSACTIONS_H
#define NEWTRANSACTIONS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include "../core/constants.h"
#include "../core/transaction.h"

namespace views{

class NewTransactions: public QFrame
{
Q_OBJECT
public:
    NewTransactions();
    NewTransactions(QWidget* parent);
    QPushButton btnSaveTransactions;
    QTableWidget tblNewTransactions;
    bool loadTransactions();
private:
    void createViewElements();
    bool checkCellFormat();
signals:
    void changedView(core::ViewNames newValue);
    void addNewTransactions(std::vector<core::Transaction>& newValue);
public slots:
    void btnSaveTransactionsClick(bool checked);
};

}

#endif // NEWTRANSACTIONS_H
