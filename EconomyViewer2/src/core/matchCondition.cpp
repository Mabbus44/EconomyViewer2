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
    case CompareProperty::ACCOUNT_NUMBER_PROPERTY:
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

std::string MatchCondition::getValueAsString() const{
    switch(_compareProperty){
    case CompareProperty::TRANSACTION_DATE_PROPERTY:
        core::Utils::toString(_valueDate);
        break;
    case CompareProperty::TRANSACTION_AMOUNT_PROPERTY:
    case CompareProperty::BALANCE_PROPERTY:
    case CompareProperty::ACCOUNT_NUMBER_PROPERTY:
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
    case CompareProperty::ACCOUNT_NUMBER_PROPERTY:
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
    case CompareProperty::ACCOUNT_NUMBER_PROPERTY:
        return ComparePropertyStr::ACCOUNT_NUMBER_PROPERTY;
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
    if(compareProperty == ComparePropertyStr::ACCOUNT_NUMBER_PROPERTY)
        return CompareProperty::ACCOUNT_NUMBER_PROPERTY;
    return CompareProperty::DESCRIPTION_PROPERTY;
}

}
