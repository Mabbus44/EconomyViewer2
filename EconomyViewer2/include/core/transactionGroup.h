#ifndef TRANSACTIONGROUP_H
#define TRANSACTIONGROUP_H

#include <map>
#include "../core/matchCondition.h"

namespace core{

class TransactionGroup
{
public:
    TransactionGroup(){}
    ~TransactionGroup(){}
    std::string accountName() {return _accountName;}
    std::string accountNumber() {return _accountNumber;}
    std::map<unsigned int, MatchCondition> matchConditions() {return _matchConditions;}
    std::map<unsigned int, MatchCondition>& matchConditionsRef() {return _matchConditions;}
    unsigned int id() {return _id;}
    void accountName(std::string accountName){_accountName = accountName;}
    void accountNumber(std::string accountNumber){_accountNumber = accountNumber;}
    void matchConditions(std::map<unsigned int, MatchCondition> matchConditions){_matchConditions = matchConditions;}
    void id(unsigned int id) {_id = id;}
    int conditionCount() {return _matchConditions.size();}
    unsigned int getUniqueMatchConditionId() {return _matchConditionId++;}
    TableRowState::TableRowState getTableRowState(const core::MatchCondition& matchCondition);
private:
    std::string _accountName;
    std::string _accountNumber;
    std::map<unsigned int, MatchCondition> _matchConditions;
    unsigned int _id = 0;
    unsigned int _matchConditionId = 1;
};
}

#endif // TRANSACTIONGROUP_H
