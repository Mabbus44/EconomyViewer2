#ifndef TRANSACTIONGROUPS_H
#define TRANSACTIONGROUPS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include "../core/constants.h"

namespace core{
class EconomyViewer;
}

namespace views{

class TransactionGroups: public QFrame
{
    Q_OBJECT
public:
    TransactionGroups();
    TransactionGroups(QWidget* parent, core::EconomyViewer* core);
    void populateTable();
    bool openView();
    QPushButton btnAddNewGroup;
    QPushButton btnDeleteGroup;
    QPushButton btnSaveChanges;
    QPushButton btnEditGroup;
    QTableWidget tblTransactionGroups;
private:
    bool checkCellFormat();
    void createViewElements();
    core::EconomyViewer* _core=nullptr;

public slots:
    void btnAddNewGroupClick(bool checked);
    void btnDeleteGroupClick(bool checked);
    void btnSaveChangesClick(bool checked);
    void btnEditGroupClick(bool checked);
};

}

#endif // TRANSACTIONGROUPS_H
