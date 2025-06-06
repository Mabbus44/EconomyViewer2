#ifndef NEWTRANSACTIONS_H
#define NEWTRANSACTIONS_H

#include <QFrame>
#include <QPushButton>
#include "../core/constants.h"

namespace views{

class NewTransactions: public QFrame
{
Q_OBJECT
public:
    NewTransactions();
    NewTransactions(QWidget* parent);
    QPushButton btnSaveTransactions;
private:
    void createViewElements();
signals:
    void changedView(core::ViewNames newValue);
public slots:
    void btnSaveTransactionsClick(bool checked);
};

}

#endif // NEWTRANSACTIONS_H
