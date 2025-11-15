#include "../../include/core/transactionGroup.h"

namespace core{
TableRowState::TableRowState TransactionGroup::getTableRowState(const core::MatchCondition& matchCondition){
    unsigned int id = matchCondition.id();
    if(_matchConditions.count(id)){
        if(_matchConditions[id] == matchCondition)
            return TableRowState::UNCHANGED;
        else
            return TableRowState::CHANGED;
    }
    return TableRowState::NEW;
}

bool TransactionGroup::isMatch(Transaction& transaction){
    for (auto& [key, val] : _matchConditions)
        if(val.isMatch(transaction))
            return true;
    return false;
}

}
