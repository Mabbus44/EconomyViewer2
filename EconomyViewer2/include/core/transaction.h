#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <ctime>
#include <string>
#include "../../include/core/jsonNode.h"

namespace core{

class Transaction
{
public:
    Transaction();
    bool operator==(const Transaction& other) const;
    void setTransactionDate(std::tm date);
    void setBalance(int balance);
    void setTransactionAmount(int transactionAmount);
    void setDescription(std::string description);
    void setFromAccount(std::string accountName);
    void setToAccount(std::string accountName);
    void setGroup(std::string group);
    void setId(unsigned int id);
    std::string getTransactionDateAsString();
    std::string getBalanceAsString();
    std::string getTransactionAmountAsString();
    std::string getIdAsString();
    std::string description();
    std::string fromAccount();
    std::string toAccount();
    std::string group();
    int balance();
    int transactionAmount();
    unsigned int getId();
    std::tm transactionDate();
    bool fromJson(JsonNode node);
    JsonNode toJson();
    void clear();
private:
    std::tm _transactionDate;
    int _balance;
    int _transactionAmount;
    unsigned int _id = 0;
    std::string _description = "";
    std::string _fromAccount = "";
    std::string _toAccount = "";
    std::string _group = "";
    bool _hasTransactionDate = false;
    bool _hasBalance = false;
    bool _hasTransactionAmount = false;
    bool _hasId = false;
    bool _hasDescription = false;
    bool _hasFromAccount = false;
    bool _hasToAccount = false;
    bool _hasGroup = false;
};

}

#endif // TRANSACTION_H
