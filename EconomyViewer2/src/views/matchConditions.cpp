#include "../../include/core/economyviewer.h"
#include "../../include/views/matchConditions.h"
#include <QSizePolicy>
#include <QLayout>

namespace views{

MatchConditions::MatchConditions() {
    createViewElements();
}

MatchConditions::MatchConditions(QWidget* parent, core::EconomyViewer* core):QFrame(parent){
    _core = core;
    createViewElements();
}

void MatchConditions::createViewElements(){
    btnAddNewMatchCondition.setParent(this);
    btnAddNewMatchCondition.setText("Add new condition");
    btnAddNewMatchCondition.setToolTip("Adds new match condition to the list");
    btnAddNewMatchCondition.show();
    btnDeleteMatchCondition.setParent(this);
    btnDeleteMatchCondition.setText("Delete condition");
    btnDeleteMatchCondition.setToolTip("Deletes selection match condition");
    btnDeleteMatchCondition.show();
    btnSaveChanges.setParent(this);
    btnSaveChanges.setText("Save changes");
    btnSaveChanges.setToolTip("Save changes to the list and return to main page");
    btnSaveChanges.show();

    tblMatchConditions.setParent(this);
    tblMatchConditions.setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    tblMatchConditions.setFixedHeight(300);
    tblMatchConditions.setFixedWidth(500);
    tblMatchConditions.show();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(&tblMatchConditions);
    layout->addWidget(&btnAddNewMatchCondition);
    layout->addWidget(&btnDeleteMatchCondition);
    layout->addWidget(&btnSaveChanges);
    this->setLayout(layout);

    this->show();
    QObject::connect(&tblMatchConditions, &QTableWidget::cellChanged, this, &MatchConditions::tblMatchConditionsChanged);
    QObject::connect(&btnAddNewMatchCondition, &QPushButton::clicked, this, &MatchConditions::btnAddNewMatchConditionClick);
    QObject::connect(&btnDeleteMatchCondition, &QPushButton::clicked, this, &MatchConditions::btnDeleteMatchConditionClick);
    QObject::connect(&btnSaveChanges, &QPushButton::clicked, this, &MatchConditions::btnSaveChangesClick);
}

bool MatchConditions::openView(core::TransactionGroup& transactionGroup){
    _transactionGroup = transactionGroup;
    tblMatchConditions.setRowCount(_transactionGroup.conditionCount());
    tblMatchConditions.setColumnCount(MatchConditionColumns::COLUMN_COUNT);
    tblMatchConditions.hideColumn(MatchConditionColumns::ID);
    tblMatchConditions.hideColumn(MatchConditionColumns::STATUS);
    tblMatchConditions.setHorizontalHeaderLabels({"Field", "Copmarator", "Value"});
    std::map<unsigned int, core::MatchCondition>& matchConditions = _transactionGroup.matchConditionsRef();
    int row = 0;
    for(auto& mapEntry: matchConditions){
        setTableRow(row, mapEntry.second);
        row++;
    }

    this->show();
    return true;
}

void MatchConditions::setTableRow(int row, const core::MatchCondition& matchCondition){
    // CompareProperty combobox
    auto comparePropertyComboBox = createComparePropertyComboBox();
    setComparePropertyComboBoxIndex(comparePropertyComboBox, matchCondition.compareProperty());
    comparePropertyComboBox->setItemData(0, row, QtRoles::Row);
    comparePropertyComboBox->setItemData(0, MatchConditionColumns::FIELD, QtRoles::Col);
    tblMatchConditions.setCellWidget(row, MatchConditionColumns::FIELD, comparePropertyComboBox);
    QObject::connect(comparePropertyComboBox, &QComboBox::currentIndexChanged, this, &MatchConditions::cmbChanged);

    // CompareType combobox
    auto compareTypeComboBox = createCompareTypeComboBox(matchCondition.compareProperty());
    setCompareTypeComboBoxIndex(compareTypeComboBox, matchCondition.compareProperty(), matchCondition.compareType());
    compareTypeComboBox->setItemData(0, row, QtRoles::Row);
    compareTypeComboBox->setItemData(0, MatchConditionColumns::COMPARATOR, QtRoles::Col);
    tblMatchConditions.setCellWidget(row, MatchConditionColumns::COMPARATOR, compareTypeComboBox);
    QObject::connect(compareTypeComboBox, &QComboBox::currentIndexChanged, this, &MatchConditions::cmbChanged);

    // The remaining columns
    tblMatchConditions.setItem(row, MatchConditionColumns::VALUE, new QTableWidgetItem(matchCondition.getValueAsString().c_str()));
    tblMatchConditions.setItem(row, MatchConditionColumns::ID, new QTableWidgetItem(std::to_string(matchCondition.id()).c_str()));
    tblMatchConditions.setItem(row, MatchConditionColumns::STATUS, new QTableWidgetItem(std::to_string(_transactionGroup.getTableRowState(matchCondition)).c_str()));
    calculateAndSetColorOfRow(row, true);
}

QComboBox* MatchConditions::createComparePropertyComboBox(){
    auto comparePropertyComboBox = new QComboBox();
    comparePropertyComboBox->addItem(ComparePropertyStr::TRANSACTION_DATE_PROPERTY, CompareProperty::TRANSACTION_DATE_PROPERTY);
    comparePropertyComboBox->addItem(ComparePropertyStr::TRANSACTION_AMOUNT_PROPERTY, CompareProperty::TRANSACTION_AMOUNT_PROPERTY);
    comparePropertyComboBox->addItem(ComparePropertyStr::BALANCE_PROPERTY, CompareProperty::BALANCE_PROPERTY);
    comparePropertyComboBox->addItem(ComparePropertyStr::DESCRIPTION_PROPERTY, CompareProperty::DESCRIPTION_PROPERTY);
    comparePropertyComboBox->addItem(ComparePropertyStr::ACCOUNT_NAME_PROPERTY, CompareProperty::ACCOUNT_NAME_PROPERTY);
    comparePropertyComboBox->addItem(ComparePropertyStr::ACCOUNT_NUMBER_PROPERTY, CompareProperty::ACCOUNT_NUMBER_PROPERTY);
    return comparePropertyComboBox;
}

QComboBox* MatchConditions::createCompareTypeComboBox(CompareProperty::CompareProperty compareProperty){
    auto compareTypeComboBox = new QComboBox();
    populateCompareTypeComboBox(compareTypeComboBox, compareProperty);
    return compareTypeComboBox;
}

void MatchConditions::populateCompareTypeComboBox(QComboBox* comboBox, CompareProperty::CompareProperty compareProperty){
    if(compareProperty == CompareProperty::DESCRIPTION_PROPERTY || compareProperty == CompareProperty::ACCOUNT_NAME_PROPERTY){
        // Strings
        comboBox->addItem(CompareTypeStr::CONTAINS, CompareType::CONTAINS);
        comboBox->addItem(CompareTypeStr::STARTS_WITH, CompareType::STARTS_WITH);
        comboBox->addItem(CompareTypeStr::ENDS_WITH, CompareType::ENDS_WITH);
        comboBox->addItem(CompareTypeStr::EQUALS, CompareType::EQUALS);
    }else{
        // Dates and ints
        comboBox->addItem(CompareTypeStr::EQUALS, CompareType::EQUALS);
        comboBox->addItem(CompareTypeStr::GRATER_THAN, CompareType::GRATER_THAN);
        comboBox->addItem(CompareTypeStr::GRATER_OR_EQUAL_TO, CompareType::GRATER_OR_EQUAL_TO);
        comboBox->addItem(CompareTypeStr::LESS_THAN, CompareType::LESS_THAN);
        comboBox->addItem(CompareTypeStr::LESS_OR_EQUAL_TO, CompareType::LESS_OR_EQUAL_TO);
    }
}

void MatchConditions::setComparePropertyComboBoxIndex(QComboBox* comboBox, CompareProperty::CompareProperty compareProperty){
    switch(compareProperty){
    default:
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        comboBox->setCurrentIndex(0);
        break;
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
        comboBox->setCurrentIndex(1);
        break;
    case CompareProperty::BALANCE_PROPERTY:
        comboBox->setCurrentIndex(2);
        break;
    case CompareProperty::DESCRIPTION_PROPERTY:
        comboBox->setCurrentIndex(3);
        break;
    case CompareProperty::ACCOUNT_NAME_PROPERTY:
        comboBox->setCurrentIndex(4);
        break;
    case CompareProperty::ACCOUNT_NUMBER_PROPERTY:
        comboBox->setCurrentIndex(5);
        break;
    }
}

void MatchConditions::setCompareTypeComboBoxIndex(QComboBox* comboBox, CompareProperty::CompareProperty compareProperty, CompareType::CompareType compareType){
    if(compareProperty == CompareProperty::DESCRIPTION_PROPERTY || compareProperty == CompareProperty::ACCOUNT_NAME_PROPERTY){
        // Strings
        switch(compareType){
        default:
        case CompareType::CONTAINS:
            comboBox->setCurrentIndex(0);
            break;
        case CompareType::STARTS_WITH:
            comboBox->setCurrentIndex(1);
            break;
        case CompareType::ENDS_WITH:
            comboBox->setCurrentIndex(2);
            break;
        case CompareType::EQUALS:
            comboBox->setCurrentIndex(3);
            break;
        }
    }else{
        // Dates and ints
        switch(compareType){
        default:
        case CompareType::EQUALS:
            comboBox->setCurrentIndex(0);
            break;
        case CompareType::GRATER_THAN:
            comboBox->setCurrentIndex(1);
            break;
        case CompareType::GRATER_OR_EQUAL_TO:
            comboBox->setCurrentIndex(2);
            break;
        case CompareType::LESS_THAN:
            comboBox->setCurrentIndex(3);
            break;
        case CompareType::LESS_OR_EQUAL_TO:
            comboBox->setCurrentIndex(4);
            break;
        }
    }
}

int MatchConditions::checkRowFormat(int row){
    std::string value = tblMatchConditions.item(row, MatchConditionColumns::VALUE)->text().toStdString();
    auto comparePropertyCmb = (QComboBox*)tblMatchConditions.cellWidget(row, MatchConditionColumns::FIELD);
    CompareProperty::CompareProperty compareProperty = (CompareProperty::CompareProperty)comparePropertyCmb->currentData().toInt();
    switch(compareProperty){
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        if(!core::Utils::isDate(value))
            return MatchConditionColumns::VALUE;
        break;
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
    case CompareProperty::BALANCE_PROPERTY:
    case CompareProperty::ACCOUNT_NUMBER_PROPERTY:
        if(!core::Utils::isNum(value))
            return MatchConditionColumns::VALUE;
        break;
    case CompareProperty::DESCRIPTION_PROPERTY:
    case CompareProperty::ACCOUNT_NAME_PROPERTY:
        break;
    }
    return -1;
}

core::MatchCondition MatchConditions::rowToMatchCondition(int row){
    core::MatchCondition m;
    QTableWidgetItem* idItemPtr = tblMatchConditions.item(row, MatchConditionColumns::ID);
    if(idItemPtr == nullptr)
        return m;
    std::string id = tblMatchConditions.item(row, MatchConditionColumns::ID)->text().toStdString();
    if(!core::Utils::isNum(id) || id == "0")
        return m;
    auto comparePropertyCmb = (QComboBox*)tblMatchConditions.cellWidget(row, MatchConditionColumns::FIELD);
    auto compareProperty = (CompareProperty::CompareProperty)comparePropertyCmb->currentData().toInt();
    auto compareTypeCmb = (QComboBox*)tblMatchConditions.cellWidget(row, MatchConditionColumns::COMPARATOR);
    auto compareType = (CompareType::CompareType)compareTypeCmb->currentData().toInt();
    std::string value = tblMatchConditions.item(row, MatchConditionColumns::VALUE)->text().toStdString();
    m.compareProperty(compareProperty);
    m.compareType(compareType);
    m.setValueWithString(value);
    m.id(core::Utils::toInt(id));
    return m;
}

void MatchConditions::cmbChanged(int index){
    if(index){} // Remove warning
    auto sender = (QComboBox*)QObject::sender();
    if(sender == nullptr)
        return;
    if(sender->count() == 0)
        return;
    int row = sender->itemData(0, QtRoles::Row).toInt();
    int col = sender->itemData(0, QtRoles::Col).toInt();
    if(col == MatchConditionColumns::FIELD){
        auto compareTypeComboBox = (QComboBox*)tblMatchConditions.cellWidget(row, MatchConditionColumns::COMPARATOR);
        if(compareTypeComboBox != nullptr){
            auto currentCompareProperty = (CompareProperty::CompareProperty)sender->currentData().toInt();
            auto currentCompareType = (CompareType::CompareType)compareTypeComboBox->currentData().toInt();
            compareTypeComboBox->blockSignals(true);
            compareTypeComboBox->clear();
            populateCompareTypeComboBox(compareTypeComboBox, currentCompareProperty);
            setCompareTypeComboBoxIndex(compareTypeComboBox, currentCompareProperty, currentCompareType);
            compareTypeComboBox->setItemData(0, row, QtRoles::Row);
            compareTypeComboBox->setItemData(0, MatchConditionColumns::COMPARATOR, QtRoles::Col);
            compareTypeComboBox->blockSignals(false);
        }
    }
    calculateAndSetColorOfRow(row);
}

void MatchConditions::tblMatchConditionsChanged(int row, int col){
    if(col){} // Remove warning
    calculateAndSetColorOfRow(row);
}

void MatchConditions::calculateAndSetColorOfRow(int row, bool alwaysSetColor){
    // Check that status colum has value (otherwise row is under construction)
    QTableWidgetItem* statusItemPtr = tblMatchConditions.item(row, MatchConditionColumns::STATUS);
    if(statusItemPtr == nullptr)
        return;

    // Compare status from column with format check of row. If it has turned to error change status column and color invalid column
    TableRowState::TableRowState stateFromTable = static_cast<TableRowState::TableRowState>(core::Utils::toInt(statusItemPtr->text().toStdString()));
    int errorCol = checkRowFormat(row);
    if(errorCol != -1){
        if(stateFromTable != TableRowState::ERROR){
            statusItemPtr->setText(std::to_string(TableRowState::ERROR).c_str());
            tblMatchConditions.item(row, errorCol)->setBackground(Qt::red);
        } else if(alwaysSetColor)
            tblMatchConditions.item(row, errorCol)->setBackground(Qt::red);
        return;
    }

    // Convert row to matchCondition, check its status and if it differs from status column, change color of row
    core::MatchCondition matchCondition = rowToMatchCondition(row);
    TableRowState::TableRowState stateFromCore = _transactionGroup.getTableRowState(matchCondition);
    if(stateFromTable != stateFromCore){
        statusItemPtr->setText(std::to_string(stateFromCore).c_str());
        changeColorOfRow(row, stateFromCore);
    } else if(alwaysSetColor)
        changeColorOfRow(row, stateFromCore);
}

void MatchConditions::changeColorOfRow(int row, TableRowState::TableRowState state){
    QBrush color = Qt::transparent;
    QString style = "";
    if(state == TableRowState::UNCHANGED){
        color = Qt::transparent;
        style = "QComboBox { background-color: transparent; }";
    }else if(state == TableRowState::CHANGED){
        color = Qt::yellow;
        style = "QComboBox { background-color: yellow; }";
    }else if(state == TableRowState::NEW){
        color = Qt::green;
        style = "QComboBox { background-color: green; }";
    }else if(state == TableRowState::ERROR){
        color = Qt::red;
        style = "QComboBox { background-color: red; }";
    }
    for(int col=0; col<MatchConditionColumns::COLUMN_COUNT; col++){
        if(col != MatchConditionColumns::ID && col != MatchConditionColumns::STATUS){
            auto item = tblMatchConditions.item(row, col);
            if(item != nullptr){
                item->setBackground(color);
            }else{
                auto widget = (QComboBox*)tblMatchConditions.cellWidget(row, col);
                if(widget != nullptr){
                    widget->setStyleSheet(style);
                }
            }
        }
    }
}

bool MatchConditions::checkAllCellsFormat(){
    int rowCount = tblMatchConditions.rowCount();
    bool ret = true;
    for(int row=0; row < rowCount; row++){
        int errorCol = checkRowFormat(row);
        if(errorCol != -1){
            tblMatchConditions.item(row, errorCol)->setBackground(Qt::red);
            ret = false;
        }
    }
    return ret;
}

void MatchConditions::btnAddNewMatchConditionClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    // Add row to table
    int rowCount = tblMatchConditions.rowCount();
    tblMatchConditions.setRowCount(rowCount+1);

    // Create new matchCondition
    core::MatchCondition newCondition;
    newCondition.compareProperty(CompareProperty::DESCRIPTION_PROPERTY);
    newCondition.compareType(CompareType::CONTAINS);
    newCondition.valueString("text");
    unsigned int id = _transactionGroup.getUniqueMatchConditionId();
    newCondition.id(id);

    // Add new matchCondiiton to row
    setTableRow(rowCount, newCondition);
}

void MatchConditions::btnDeleteMatchConditionClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    QList<QTableWidgetItem*> selectedItems = tblMatchConditions.selectedItems();
    if(selectedItems.size() == 0)
        return;
    tblMatchConditions.removeRow(selectedItems[0]->row());
}

void MatchConditions::btnSaveChangesClick(bool checked){
    if(checked){}     //Remove warning "checked unused"
    if(checkAllCellsFormat()){
        int rowCount = tblMatchConditions.rowCount();
        std::map<unsigned int, core::MatchCondition> matchConditions;
        for(int row=0; row < rowCount; row++){
            core::MatchCondition newMatchCondition = rowToMatchCondition(row);
            matchConditions[newMatchCondition.id()] = newMatchCondition;
        }
        _transactionGroup.matchConditions(matchConditions);
        std::map<unsigned int, core::TransactionGroup> sigleTransactionGroup;
        sigleTransactionGroup[_transactionGroup.id()] = _transactionGroup;
        _core->openTransactionGroupsView(sigleTransactionGroup);
    }
}

}
