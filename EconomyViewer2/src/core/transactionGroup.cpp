#include "../../include/core/transactionGroup.h"

namespace core{
bool TransactionGroup::operator==(const TransactionGroup& other) const{
    if(this->_groupName != other._groupName)
        return false;
    if(this->_id != other._id)
        return false;
    if(this->_matchConditionId != other._matchConditionId)
        return false;
    if(_matchConditions.size() != other._matchConditions.size())
        return false;
    for(auto& [key, matchCondition]: _matchConditions){
        if(!other._matchConditions.count(key))
            return false;
        if(other._matchConditions.at(key) != matchCondition)
            return false;
    }
    return true;
}

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

bool TransactionGroup::fromJson(JsonNode node){
    if(node.type() != JsonNodeType::OBJECT)
        return false;
    clear();
    auto& obj = node.objectRef();

    if(obj.count("_groupName")){
        auto groupName = *obj["_groupName"];
        if(groupName.type() != JsonNodeType::VALUE)
            return false;
        _groupName = groupName.value();
    }else
        return false;
    if(obj.count("_matchConditions")){
        auto matchConditionsNode = *obj["_matchConditions"];
        if(matchConditionsNode.type() != JsonNodeType::OBJECT){
        }else{
            auto& matchConditions = matchConditionsNode.objectRef();
            for(auto& [key, matchConditionsNodePtr]: matchConditions){
                MatchCondition matchCondition;
                if(matchCondition.fromJson(*matchConditionsNodePtr))
                    _matchConditions[matchCondition.id()] = matchCondition;
            }
        }
    }
    if(obj.count("_id")){
        auto id = *obj["_id"];
        if(id.type() != JsonNodeType::VALUE)
            return false;
        _id = id.valueAsInt();
    }else
        return false;
    if(obj.count("_matchConditionId")){
        auto matchConditionId = *obj["_matchConditionId"];
        if(matchConditionId.type() != JsonNodeType::VALUE)
            return false;
        _matchConditionId = matchConditionId.valueAsInt();
    }else
        return false;
    return true;
}

JsonNode TransactionGroup::toJson(){
    JsonNode node, matchConditionNodes;
    node.addToObject("_groupName", _groupName);
    for(auto& [id, matchCondition]: _matchConditions)
        matchConditionNodes.addToObject(std::to_string(id), matchCondition.toJson());
    node.addToObject("_matchConditions", matchConditionNodes);
    node.addToObject("_id", std::to_string(_id));
    node.addToObject("_matchConditionId", std::to_string(_matchConditionId));
    return node;
}

}
