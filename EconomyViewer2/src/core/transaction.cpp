#include "../../include/core/transaction.h"
#include "../../include/core/constants.h"
#include <format>

namespace core{

Transaction::Transaction() {}

bool Transaction::operator==(const Transaction& other) const{
    if(this->_hasTransactionDate != other._hasTransactionDate)
        return false;
    if(this->_hasTransactionDate && Utils::compareDates(this->_transactionDate, other._transactionDate) != DateComp::SAME_TIME)
        return false;

    if(this->_hasBalance != other._hasBalance)
        return false;
    if(this->_hasBalance && this->_balance != other._balance)
        return false;

    if(this->_hasTransactionAmount != other._hasTransactionAmount)
        return false;
    if(this->_hasTransactionAmount && this->_transactionAmount != other._transactionAmount)
        return false;

    if(this->_hasId != other._hasId)
        return false;
    if(this->_id != other._id)
        return false;

    if(this->_hasDescription != other._hasDescription)
        return false;
    if(this->_hasDescription && this->_description != other._description)
        return false;

    if(this->_hasFromAccount != other._hasFromAccount)
        return false;
    if(this->_fromAccount != other._fromAccount)
        return false;

    if(this->_hasToAccount != other._hasToAccount)
        return false;
    if(this->_toAccount != other._toAccount)
        return false;

    if(this->_hasGroup != other._hasGroup)
        return false;
    if(this->_group != other._group)
        return false;

    return true;
}

void Transaction::setTransactionDate(std::tm date){
    _transactionDate = date;
    _hasTransactionDate = true;
}

void Transaction::setBalance(int balance){
    _balance = balance;
    _hasBalance = true;
}

void Transaction::setTransactionAmount(int transactionAmount){
    _transactionAmount = transactionAmount;
    _hasTransactionAmount = true;
}

void Transaction::setDescription(std::string description){
    _description = description;
    _hasDescription = true;
}

void Transaction::setFromAccount(std::string accountName){
    _hasFromAccount = true;
    _fromAccount = accountName;
}

void Transaction::setToAccount(std::string accountName){
    _hasToAccount = true;
    _toAccount = accountName;
}

void Transaction::setGroup(std::string group){
    _hasGroup = true;
    _group = group;
}

void Transaction::setId(unsigned int id){
    _hasId = true;
    _id = id;
}

std::string Transaction::getTransactionDateAsString(){
    if(_hasTransactionDate){
        int year = _transactionDate.tm_year + 1900;
        int month = _transactionDate.tm_mon + 1;
        int day = _transactionDate.tm_mday;
        std::string ret = std::format("{:04}-{:02}-{:02}", year, month, day);
        return ret;
    }
    return "";
}

std::string Transaction::getBalanceAsString(){
    if(_hasBalance)
        return std::to_string(_balance);
    return "";
}

std::string Transaction::getTransactionAmountAsString(){
    if(_hasTransactionAmount)
        return std::to_string(_transactionAmount);
    return "";
}

std::string Transaction::description(){
    if(_hasDescription)
        return _description;
    return "";
}

std::string Transaction::fromAccount(){
    if(_hasFromAccount)
        return _fromAccount;
    return "";
}

std::string Transaction::toAccount(){
    if(_hasToAccount)
        return _toAccount;
    return "";
}

std::string Transaction::group(){
    if(_hasGroup)
        return _group;
    return "";
}

std::string Transaction::getIdAsString(){
    if(_hasId)
        return std::to_string(_id);
    return "";
}

int Transaction::balance(){
    if(_hasBalance)
        return _balance;
    return 0;
}

int Transaction::transactionAmount(){
    if(_hasTransactionAmount)
        return _transactionAmount;
    return 0;
}

unsigned int Transaction::getId(){
    if(_hasId)
        return _id;
    return 0;
}

std::tm Transaction::transactionDate(){
    if(_hasTransactionDate)
        return _transactionDate;
    std::tm ret;
    return ret;
}

bool Transaction::fromJson(JsonNode node){
    if(node.type() != JsonNodeType::OBJECT)
        return false;
    clear();
    auto& obj = node.objectRef();

    if(obj.count("_hasTransactionDate") && obj.count("_transactionDate")){
        auto hasTransactionDate = *obj["_hasTransactionDate"];
        auto transactionDate = *obj["_transactionDate"];
        if(hasTransactionDate.type() != JsonNodeType::VALUE || transactionDate.type() != JsonNodeType::VALUE){
            _hasTransactionDate = false;
        }else{
            _hasTransactionDate = hasTransactionDate.valueAsBool();
            _transactionDate = Utils::toDate(transactionDate.value());
        }
    }else{
        _hasTransactionDate = false;
    }
    if(obj.count("_hasBalance") && obj.count("_balance")){
        auto hasBalance = *obj["_hasBalance"];
        auto balance = *obj["_balance"];
        if(hasBalance.type() != JsonNodeType::VALUE || balance.type() != JsonNodeType::VALUE){
            _hasBalance = false;
        }else{
            _hasBalance = hasBalance.valueAsBool();
            _balance = balance.valueAsInt();
        }
    }else{
        _hasBalance = false;
    }
    if(obj.count("_hasTransactionAmount") && obj.count("_transactionAmount")){
        auto hasTransactionAmount = *obj["_hasTransactionAmount"];
        auto transactionAmount = *obj["_transactionAmount"];
        if(hasTransactionAmount.type() != JsonNodeType::VALUE || transactionAmount.type() != JsonNodeType::VALUE){
            _hasTransactionAmount = false;
        }else{
            _hasTransactionAmount = hasTransactionAmount.valueAsBool();
            _transactionAmount = transactionAmount.valueAsInt();
        }
    }else{
        _hasTransactionAmount = false;
    }
    if(obj.count("_hasId") && obj.count("_id")){
        auto hasId = *obj["_hasId"];
        auto id = *obj["_id"];
        if(hasId.type() != JsonNodeType::VALUE || id.type() != JsonNodeType::VALUE){
            _hasId = false;
            return false;
        }else{
            _hasId = hasId.valueAsBool();
            _id = id.valueAsInt();
        }
    }else{
        _hasId = false;
        return false;
    }
    if(obj.count("_hasDescription") && obj.count("_description")){
        auto hasDescription = *obj["_hasDescription"];
        auto description = *obj["_description"];
        if(hasDescription.type() != JsonNodeType::VALUE || description.type() != JsonNodeType::VALUE){
            _hasDescription = false;
        }else{
            _hasDescription = hasDescription.valueAsBool();
            _description = description.value();
        }
    }else{
        _hasDescription = false;
    }
    if(obj.count("_hasFromAccount") && obj.count("_fromAccount")){
        auto hasFromAccount = *obj["_hasFromAccount"];
        auto fromAccount = *obj["_fromAccount"];
        if(hasFromAccount.type() != JsonNodeType::VALUE || fromAccount.type() != JsonNodeType::VALUE){
            _hasFromAccount = false;
        }else{
            _hasFromAccount = hasFromAccount.valueAsBool();
            _fromAccount = fromAccount.value();
        }
    }else{
        _hasFromAccount = false;
    }
    if(obj.count("_hasToAccount") && obj.count("_toAccount")){
        auto hasToAccount = *obj["_hasToAccount"];
        auto toAccount = *obj["_toAccount"];
        if(hasToAccount.type() != JsonNodeType::VALUE || toAccount.type() != JsonNodeType::VALUE){
            _hasToAccount = false;
        }else{
            _hasToAccount = hasToAccount.valueAsBool();
            _toAccount = toAccount.value();
        }
    }else{
        _hasToAccount = false;
    }
    if(obj.count("_hasGroup") && obj.count("_group")){
        auto hasGroup = *obj["_hasGroup"];
        auto group = *obj["_group"];
        if(hasGroup.type() != JsonNodeType::VALUE || group.type() != JsonNodeType::VALUE){
            _hasGroup = false;
        }else{
            _hasGroup = hasGroup.valueAsBool();
            _group = group.value();
        }
    }else{
        _hasGroup = false;
    }
    return true;
}

JsonNode Transaction::toJson(){
    JsonNode node;
    node.addToObject("_hasTransactionDate", Utils::toString(_hasTransactionDate));
    node.addToObject("_hasBalance", Utils::toString(_hasBalance));
    node.addToObject("_hasTransactionAmount", Utils::toString(_hasTransactionAmount));
    node.addToObject("_hasId", Utils::toString(_hasId));
    node.addToObject("_hasDescription", Utils::toString(_hasDescription));
    node.addToObject("_hasFromAccount", Utils::toString(_hasFromAccount));
    node.addToObject("_hasToAccount", Utils::toString(_hasToAccount));
    node.addToObject("_hasGroup", Utils::toString(_hasGroup));
    if(_hasTransactionDate)
        node.addToObject("_transactionDate", Utils::toString(_transactionDate));
    if(_hasBalance)
        node.addToObject("_balance", std::to_string(_balance));
    if(_hasTransactionAmount)
        node.addToObject("_transactionAmount", std::to_string(_transactionAmount));
    if(_hasId)
        node.addToObject("_id", std::to_string(_id));
    if(_hasDescription)
        node.addToObject("_description", _description);
    if(_hasDescription)
        node.addToObject("_fromAccount", _fromAccount);
    if(_hasToAccount)
        node.addToObject("_toAccount", _toAccount);
    if(_hasGroup)
        node.addToObject("_group", _group);
    return node;
}

void Transaction::clear() {
    _id = 0;
    _hasTransactionDate = false;
    _hasBalance = false;
    _hasTransactionAmount = false;
    _hasId = false;
    _hasDescription = false;
    _hasFromAccount = false;
    _hasToAccount = false;
    _hasGroup = false;
}

}
