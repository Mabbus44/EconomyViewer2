#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>

namespace core{

class Account
{
public:
    Account(){}
    ~Account(){}
    void name(std::string s){_name = s;}
    void accountNumber(std::string s){_accountNumber = s;}
    std::string name(){return _name;}
    std::string accountNumber(){return _accountNumber;}
private:
    std::string _name;
    std::string _accountNumber;
};
}

#endif // ACCOUNT_H
