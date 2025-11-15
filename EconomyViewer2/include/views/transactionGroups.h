#ifndef TRANSACTIONGROUPS_H
#define TRANSACTIONGROUPS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include "../core/transactionGroup.h"

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
    bool openView();
    bool openView(std::map<unsigned int, core::TransactionGroup>& transactionGroups, bool appendTransactionGroups = false);
    QPushButton btnAddNewGroup;
    QPushButton btnDeleteGroup;
    QPushButton btnCancel;
    QPushButton btnSaveChanges;
    QPushButton btnEditGroup;
    QTableWidget tblTransactionGroups;
private:
    void createViewElements();
    core::EconomyViewer* _core=nullptr;
    std::map<unsigned int, core::TransactionGroup> _transactionGroups;

public slots:
    void btnAddNewGroupClick(bool checked);
    void btnDeleteGroupClick(bool checked);
    void btnCancelClick(bool checked);
    void btnSaveChangesClick(bool checked);
    void btnEditGroupClick(bool checked);
};

}

#endif // TRANSACTIONGROUPS_H
