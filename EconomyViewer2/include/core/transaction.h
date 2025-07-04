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
    std::string getTransactionDateAsString();
    std::string getBalanceAsString();
    std::string getTransactionAmountAsString();
    std::string getDescriptionAsString();
private:
    std::tm _transactionDate;
    int _balance;   // Balance after transaction
    int _transactionAmount;
    std::string _description;
    bool _hasTransactionDate = false;
    bool _hasBalance = false;
    bool _hasTransactionAmount = false;
    bool _hasDescription = false;
};

}

#endif // TRANSACTION_H
