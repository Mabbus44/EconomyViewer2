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
    bool openView(std::map<unsigned int, core::Transaction>& transactions, bool appendTransactions = false);
    bool openView(std::vector<core::Transaction>& transactions, bool appendTransactions = false);
    QPushButton btnAddTransactions;
    QPushButton btnManageAccounts;
    QPushButton btnApplyChanges;
    QTableWidget tblTransactions;
private:
    void createViewElements();
    bool checkAllCellsFormat();
    void updateAllCellsFormat();
    int checkRowFormat(int row);
    bool saveTransactionsToCore();
    void calculateAndSetColorOfRow(int row, bool alwaysSetColor=false);
    void changeColorOfRow(int row, core::TransactionState state);
    core::Transaction rowToTransaction(int row);
    core::EconomyViewer* _core=nullptr;

public slots:
    void tblTransactionsChanged(int row, int col);
    void btnAddTransactionsClick(bool checked);
    void btnManageAccountsClick(bool checked);
    void btnApplyChangesClick(bool checked);

};

}

#endif // TRANSACTIONS_H
