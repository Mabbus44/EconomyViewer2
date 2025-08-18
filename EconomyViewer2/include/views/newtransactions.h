#ifndef NEWTRANSACTIONS_H
#define NEWTRANSACTIONS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include "../core/constants.h"
#include "../core/transaction.h"
#include "../core/account.h"

namespace core{
class EconomyViewer;
}

namespace views{

class NewTransactions: public QFrame
{
Q_OBJECT
public:
    NewTransactions();
    NewTransactions(QWidget* parent, core::EconomyViewer* core);
    QPushButton btnSaveTransactions;
    QTableWidget tblNewTransactions;
    QComboBox cmbAccounts;
    bool openView(std::vector<core::Account>& accounts, bool loadFileDialog = true);
    bool openView(bool loadFileDialog = true);
private:
    void createViewElements();
    bool checkCellFormat();
    bool loadTransactions();
    bool loadAccounts(std::vector<core::Account>& accounts);
    core::EconomyViewer* _core=nullptr;
public slots:
    void btnSaveTransactionsClick(bool checked);
};

}

#endif // NEWTRANSACTIONS_H
