#include "../../include/core/account.h"

namespace core{

bool Account::fromJson(JsonNode node){
    if(node.type() != JsonNodeType::OBJECT)
        return false;
    clear();
    auto& obj = node.objectRef();

    if(obj.count("_name")){
        auto name = *obj["_name"];
        if(name.type() != JsonNodeType::VALUE)
            return false;
        _name = name.value();
    }else
        return false;
    if(obj.count("_accountNumber")){
        auto accountNumber = *obj["_accountNumber"];
        if(accountNumber.type() != JsonNodeType::VALUE)
            return false;
        _accountNumber = accountNumber.value();
    }else
        return false;
    return true;
}

JsonNode Account::toJson(){
    JsonNode node;
    node.addToObject("_name", _name);
    node.addToObject("_accountNumber", _accountNumber);
    return node;
}

}
