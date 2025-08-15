#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <ctime>
#include <string>

namespace core{

class Transaction
{
public:
    Transaction();
    void setTransactionDate(std::tm date);
    void setBalance(int balance);
    void setTransactionAmount(int transactionAmount);
    void setDescription(std::string description);
    void setFromAccount(std::string accountName);
    void setToAccount(std::string accountName);
    std::string getTransactionDateAsString();
    std::string getBalanceAsString();
    std::string getTransactionAmountAsString();
    std::string getDescriptionAsString();
    std::string getFromAccountAsString();
    std::string getToAccountAsString();
private:
    std::tm _transactionDate;
    int _balance;   // Balance after transaction
    int _transactionAmount;
    std::string _description;
    std::string _fromAccount = "";
    std::string _toAccount = "";
    bool _hasTransactionDate = false;
    bool _hasBalance = false;
    bool _hasTransactionAmount = false;
    bool _hasDescription = false;
};

}

#endif // TRANSACTION_H
