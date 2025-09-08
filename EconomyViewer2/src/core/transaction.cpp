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

    if(this->_hasDescription != other._hasDescription)
        return false;
    if(this->_hasDescription && this->_description != other._description)
        return false;

    if(this->_id != other._id)
        return false;
    if(this->_fromAccount != other._fromAccount)
        return false;
    if(this->_toAccount != other._toAccount)
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
    _fromAccount = accountName;
}

void Transaction::setToAccount(std::string accountName){
    _toAccount = accountName;
}

void Transaction::setId(unsigned int id){
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

std::string Transaction::getDescriptionAsString(){
    if(_hasDescription)
        return _description;
    return "";
}

std::string Transaction::getFromAccountAsString(){
    return _fromAccount;
}

std::string Transaction::getToAccountAsString(){
    return _toAccount;
}

std::string Transaction::getIdAsString(){
    return std::to_string(_id);
}

unsigned int Transaction::getId(){
    return _id;
}
}

