#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include "../core/constants.h"
#include "../core/account.h"

namespace core{
class EconomyViewer;
}

namespace views{

class Accounts: public QFrame
{
    Q_OBJECT
public:
    Accounts();
    Accounts(QWidget* parent, core::EconomyViewer* core);
    void populateTable();
    QPushButton btnAddNewAccount;
    QPushButton btnDeleteAccount;
    QPushButton btnSaveChanges;
    QTableWidget tblAccounts;
    bool openView();
    bool openView(std::vector<core::Account>& accounts);
private:
    bool checkCellFormat();
    void createViewElements();
    core::EconomyViewer* _core=nullptr;

public slots:
    void btnAddNewAccountClick(bool checked);
    void btnDeleteAccountClick(bool checked);
    void btnSaveChangesClick(bool checked);

};

}
#endif // ACCOUNTS_H
