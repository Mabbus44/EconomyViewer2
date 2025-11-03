#ifndef MATCHCONDITIONS_H
#define MATCHCONDITIONS_H

#include <QFrame>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include "../core/transactionGroup.h"

namespace core{
class EconomyViewer;
}

namespace views{

class MatchConditions: public QFrame
{
    Q_OBJECT
public:
    MatchConditions();
    MatchConditions(QWidget* parent, core::EconomyViewer* core);
    bool openView(core::TransactionGroup& transactionGroup);
    QPushButton btnAddNewMatchCondition;
    QPushButton btnDeleteMatchCondition;
    QPushButton btnSaveChanges;
    QTableWidget tblMatchConditions;
private:
    void createViewElements();
    void setTableRow(int row, const core::MatchCondition& matchCondition);
    QComboBox* createComparePropertyComboBox();
    QComboBox* createCompareTypeComboBox(CompareProperty::CompareProperty compareProperty);
    void populateCompareTypeComboBox(QComboBox* comboBox, CompareProperty::CompareProperty compareProperty);
    void setComparePropertyComboBoxIndex(QComboBox* comboBox, CompareProperty::CompareProperty compareProperty);
    void setCompareTypeComboBoxIndex(QComboBox* comboBox, CompareProperty::CompareProperty compareProperty, CompareType::CompareType compareType);
    int checkRowFormat(int row);
    void calculateAndSetColorOfRow(int row, bool alwaysSetColor=false);
    void changeColorOfRow(int row, TableRowState::TableRowState state);
    bool checkAllCellsFormat();
    core::MatchCondition rowToMatchCondition(int row);
    core::EconomyViewer* _core=nullptr;
    core::TransactionGroup _transactionGroup;

public slots:
    void cmbChanged(int index);
    void tblMatchConditionsChanged(int row, int col);
    void btnAddNewMatchConditionClick(bool checked);
    void btnDeleteMatchConditionClick(bool checked);
    void btnSaveChangesClick(bool checked);
};

}

#endif // MATCHCONDITIONS_H
