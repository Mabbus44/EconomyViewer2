#ifndef TRANSACTIONGROUP_H
#define TRANSACTIONGROUP_H

#include <map>
#include "../core/matchCondition.h"
#include "../../include/core/jsonNode.h"

namespace core{

class TransactionGroup
{
public:
    TransactionGroup(){}
    ~TransactionGroup(){}
    bool operator==(const TransactionGroup& other) const;
    std::string groupName() {return _groupName;}
    unsigned int id() {return _id;}
    unsigned int getUniqueMatchConditionId() {return _matchConditionId++;}
    int conditionCount() {return _matchConditions.size();}
    bool isMatch(Transaction& transaction);
    void groupName(std::string groupName){_groupName = groupName;}
    void matchConditions(std::map<unsigned int, MatchCondition> matchConditions){_matchConditions = matchConditions;}
    void id(unsigned int id) {_id = id;}
    TableRowState::TableRowState getTableRowState(const core::MatchCondition& matchCondition);
    std::map<unsigned int, MatchCondition> matchConditions() {return _matchConditions;}
    std::map<unsigned int, MatchCondition>& matchConditionsRef() {return _matchConditions;}
    bool fromJson(JsonNode node);
    JsonNode toJson();
    void clear() {_matchConditions.clear(); _id = 0; _matchConditionId = 1;};
private:
    std::string _groupName;
    std::map<unsigned int, MatchCondition> _matchConditions;
    unsigned int _id = 0;
    unsigned int _matchConditionId = 1;
};
}

#endif // TRANSACTIONGROUP_H
