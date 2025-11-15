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
}

