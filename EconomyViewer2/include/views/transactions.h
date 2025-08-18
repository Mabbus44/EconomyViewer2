#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include "../core/constants.h"
#include "../core/transaction.h"

namespace core{
class EconomyViewer;
}

namespace views{

class Transactions: public QFrame
{
Q_OBJECT
public:
    Transactions();
    Transactions(QWidget* parent, core::EconomyViewer* core);
    bool openView();
    bool openView(std::vector<core::Transaction>& transactions, bool appendTransactions = false);
    QPushButton btnAddTransactions;
    QPushButton btnManageAccounts;
    QPushButton btnApplyChanges;
    QTableWidget tblTransactions;
private:
    void createViewElements();
    bool checkCellFormat();
    bool saveTransactionsToCore();
    core::EconomyViewer* _core=nullptr;

public slots:
    void btnAddTransactionsClick(bool checked);
    void btnManageAccountsClick(bool checked);
    void btnApplyChangesClick(bool checked);

};

}

#endif // TRANSACTIONS_H
