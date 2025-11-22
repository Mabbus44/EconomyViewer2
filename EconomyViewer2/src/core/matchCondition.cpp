#include "../../include/core/matchCondition.h"

namespace core{

bool MatchCondition::operator==(const MatchCondition& other) const{
    if(this->_compareType != other._compareType)
        return false;
    if(this->_compareProperty != other._compareProperty)
        return false;
    switch(this->_compareProperty){
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        if(Utils::compareDates(this->_valueDate, other._valueDate) != DateComp::SAME_TIME)
            return false;
        break;
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
    case CompareProperty::BALANCE_PROPERTY:
        if(this->_valueInt != other._valueInt)
            return false;
        break;
    case CompareProperty::DESCRIPTION_PROPERTY:
    case CompareProperty::ACCOUNT_NAME_PROPERTY:
        if(this->_valueString != other._valueString)
            return false;
        break;
    }
    return true;
}

bool MatchCondition::isMatch(Transaction& transaction){
    switch(_compareProperty){
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        return isDateMatch(transaction.transactionDate());
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
        return isIntMatch(transaction.transactionAmount());
    case CompareProperty::BALANCE_PROPERTY:
        return isIntMatch(transaction.balance());
    case CompareProperty::DESCRIPTION_PROPERTY:
        return isStringMatch(transaction.description());
    case CompareProperty::ACCOUNT_NAME_PROPERTY:
        return isStringMatch(transaction.fromAccount()) || isStringMatch(transaction.toAccount());
    }
    return false;
}

bool MatchCondition::isDateMatch(std::tm date){
    DateComp::DateComp relation = core::Utils::compareDates(date, _valueDate);
    switch(_compareType){
    case CompareType::GRATER_THAN:
        return relation == DateComp::LATER;
    case CompareType::LESS_THAN:
        return relation == DateComp::EARLIER;
    case CompareType::GRATER_OR_EQUAL_TO:
        return relation == DateComp::LATER || relation == DateComp::SAME_TIME;
    case CompareType::LESS_OR_EQUAL_TO:
        return relation == DateComp::EARLIER || relation == DateComp::SAME_TIME;
    case CompareType::EQUALS:
        return relation == DateComp::SAME_TIME;
    default:
        return false;
    }
}

bool MatchCondition::isStringMatch(std::string str){
    str = core::Utils::toLower(str);
    std::string val = core::Utils::toLower(_valueString);
    switch(_compareType){
    case CompareType::CONTAINS:
        return str.find(val) != std::string::npos;
    case CompareType::STARTS_WITH:
        return str.starts_with(val);
    case CompareType::ENDS_WITH:
        return str.ends_with(val);
    case CompareType::EQUALS:
        return str == val;
    default:
        return false;
    }
}

bool MatchCondition::isIntMatch(int num){
    switch(_compareType){
    case CompareType::GRATER_THAN:
        return num > _valueInt;
    case CompareType::LESS_THAN:
        return num < _valueInt;
    case CompareType::GRATER_OR_EQUAL_TO:
        return num >= _valueInt;
    case CompareType::LESS_OR_EQUAL_TO:
        return num <= _valueInt;
    case CompareType::EQUALS:
        return num == _valueInt;
    default:
        return false;
    }
}

std::string MatchCondition::getValueAsString() const{
    switch(_compareProperty){
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        core::Utils::toString(_valueDate);
        break;
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
    case CompareProperty::BALANCE_PROPERTY:
        return std::to_string((_valueInt));
        break;
    case CompareProperty::DESCRIPTION_PROPERTY:
    case CompareProperty::ACCOUNT_NAME_PROPERTY:
        return _valueString;
        break;
    }
    return "";
}

void MatchCondition::setValueWithString(std::string value){
    switch(_compareProperty){
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        _valueDate = core::Utils::toDate(value);
        break;
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
    case CompareProperty::BALANCE_PROPERTY:
        _valueInt = core::Utils::toInt(value);
        break;
    case CompareProperty::DESCRIPTION_PROPERTY:
    case CompareProperty::ACCOUNT_NAME_PROPERTY:
        _valueString = value;
        break;
    }
}

std::string MatchCondition::compareTypeToString(CompareType::CompareType compareType){
    switch(compareType){
    case CompareType::CONTAINS:
        return CompareTypeStr::CONTAINS;
        break;
    case CompareType::STARTS_WITH:
        return CompareTypeStr::STARTS_WITH;
        break;
    case CompareType::ENDS_WITH:
        return CompareTypeStr::ENDS_WITH;
        break;
    case CompareType::GRATER_THAN:
        return CompareTypeStr::GRATER_THAN;
        break;
    case CompareType::LESS_THAN:
        return CompareTypeStr::LESS_THAN;
        break;
    case CompareType::GRATER_OR_EQUAL_TO:
        return CompareTypeStr::GRATER_OR_EQUAL_TO;
        break;
    case CompareType::LESS_OR_EQUAL_TO:
        return CompareTypeStr::LESS_OR_EQUAL_TO;
        break;
    case CompareType::EQUALS:
        return CompareTypeStr::EQUALS;
        break;
    }
    return "error";
}

CompareType::CompareType MatchCondition::stringToCompareType(std::string compareType){
    if(compareType == CompareTypeStr::CONTAINS)
        return CompareType::CONTAINS;
    if(compareType == CompareTypeStr::STARTS_WITH)
        return CompareType::STARTS_WITH;
    if(compareType == CompareTypeStr::ENDS_WITH)
        return CompareType::ENDS_WITH;
    if(compareType == CompareTypeStr::GRATER_THAN)
        return CompareType::GRATER_THAN;
    if(compareType == CompareTypeStr::LESS_THAN)
        return CompareType::LESS_THAN;
    if(compareType == CompareTypeStr::GRATER_OR_EQUAL_TO)
        return CompareType::GRATER_OR_EQUAL_TO;
    if(compareType == CompareTypeStr::LESS_OR_EQUAL_TO)
        return CompareType::LESS_OR_EQUAL_TO;
    if(compareType == CompareTypeStr::EQUALS)
        return CompareType::EQUALS;
    return CompareType::EQUALS;
}

std::string MatchCondition::comparePropertyToString(CompareProperty::CompareProperty compareProperty){
    switch(compareProperty){
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        return ComparePropertyStr::TRANSACTION_DATE_PROPERTY;
        break;
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
        return ComparePropertyStr::TRANSACTION_AMOUNT_PROPERTY;
        break;
    case CompareProperty::BALANCE_PROPERTY:
        return ComparePropertyStr::BALANCE_PROPERTY;
        break;
    case CompareProperty::DESCRIPTION_PROPERTY:
        return ComparePropertyStr::DESCRIPTION_PROPERTY;
        break;
    case CompareProperty::ACCOUNT_NAME_PROPERTY:
        return ComparePropertyStr::ACCOUNT_NAME_PROPERTY;
        break;
    }
    return "error";
}

CompareProperty::CompareProperty MatchCondition::stringToCompareProperty(std::string compareProperty){
    if(compareProperty == ComparePropertyStr::TRANSACTION_DATE_PROPERTY)
        return CompareProperty::TRANSACTION_DATE_PROPERTY;
    if(compareProperty == ComparePropertyStr::TRANSACTION_AMOUNT_PROPERTY)
        return CompareProperty::TRANSACTION_AMOUNT_PROPERTY;
    if(compareProperty == ComparePropertyStr::BALANCE_PROPERTY)
        return CompareProperty::BALANCE_PROPERTY;
    if(compareProperty == ComparePropertyStr::DESCRIPTION_PROPERTY)
        return CompareProperty::DESCRIPTION_PROPERTY;
    if(compareProperty == ComparePropertyStr::ACCOUNT_NAME_PROPERTY)
        return CompareProperty::ACCOUNT_NAME_PROPERTY;
    return CompareProperty::DESCRIPTION_PROPERTY;
}

bool MatchCondition::fromJson(JsonNode node){
    if(node.type() != JsonNodeType::OBJECT)
        return false;
    clear();
    auto& obj = node.objectRef();

    if(obj.count("_compareType")){
        auto node = *obj["_compareType"];
        if(node.type() != JsonNodeType::VALUE)
            return false;
        _compareType = stringToCompareType(node.value());
    }else
        return false;
    if(obj.count("_compareProperty")){
        auto node = *obj["_compareProperty"];
        if(node.type() != JsonNodeType::VALUE)
            return false;
        _compareProperty = stringToCompareProperty(node.value());
    }else
        return false;
    if(obj.count("_valueDate")){
        auto node = *obj["_valueDate"];
        if(node.type() != JsonNodeType::VALUE){
        }else
            _valueDate = Utils::toDate(node.value());
    }
    if(obj.count("_valueString")){
        auto node = *obj["_valueString"];
        if(node.type() != JsonNodeType::VALUE){
        }else
            _valueString = node.value();
    }
    if(obj.count("_valueInt")){
        auto node = *obj["_valueInt"];
        if(node.type() != JsonNodeType::VALUE){
        }else
            _valueInt = node.valueAsInt();
    }
    if(obj.count("_id")){
        auto id = *obj["_id"];
        if(id.type() != JsonNodeType::VALUE)
            return false;
        _id = id.valueAsInt();
    }else
        return false;
    return true;
}

JsonNode MatchCondition::toJson(){
    JsonNode node;
    node.addToObject("_compareType", compareTypeToString(_compareType));
    node.addToObject("_compareProperty", comparePropertyToString(_compareProperty));
    node.addToObject("_id", std::to_string(_id));
    switch(_compareProperty){
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        node.addToObject("_valueDate", Utils::toString(_valueDate));
        break;
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
    case CompareProperty::BALANCE_PROPERTY:
        node.addToObject("_valueInt", std::to_string(_valueInt));
        break;
    case CompareProperty::DESCRIPTION_PROPERTY:
    case CompareProperty::ACCOUNT_NAME_PROPERTY:
        node.addToObject("_valueString", _valueString);
        break;
    }
    return node;
}

}
