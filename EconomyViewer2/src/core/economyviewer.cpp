#include "../../include/core/economyviewer.h"
#include "../../include/core/constants.h"
#include "../../include/core/jsonNode.h"
#include <QMessageBox>
#include <fstream>

namespace core{

EconomyViewer::EconomyViewer() {
    /* Test passed, save it until next git commit just in case
    JsonNode root, hits, total, hitsArray, hitsArray1, source, cats, catNames;
    catNames.addToArray("Möbler");
    catNames.addToArray("Stol");
    catNames.addToArray("Elevstol");
    cats.addToArray("44212");
    cats.addToArray("44210");
    cats.addToArray("44160");
    source.addToObject("agreementProposalSortOrder", "0");
    source.addToObject("categories", cats);
    source.addToObject("categoryNames", catNames);
    source.addToObject("description", "Elevstol Leon 1, högtryckslaminat ");
    source.addToObject("entityId", "44872");
    hitsArray1.addToObject("_index", "ecom_dev_product_sv_se_202511080651");
    hitsArray1.addToObject("_type", "_doc");
    hitsArray1.addToObject("_id", "P7055030_7055030");
    hitsArray1.addToObject("_score", "1.0");
    hitsArray1.addToObject("_source", source);
    hitsArray.addToArray(hitsArray1);
    total.addToObject("value", "4");
    total.addToObject("relation", "eq");
    hits.addToObject("total", total);
    hits.addToObject("max_score", "1.0");
    hits.addToObject("hits", hitsArray);
    root.addToObject("hits", hits);
    std::string json = root.getJson();*/
}

int EconomyViewer::run(int argc, char **argv) {
    QApplication app (argc, argv);
    window.reset(new QFrame());
    window->resize(600, 800);
    window->show();

    transactionsView.reset(new views::Transactions(window.get(), this));
    transactionsView->openView();
    currentView = ViewNames::TRANSACTIONS;

    newTransactionsView.reset(new views::NewTransactions(window.get(), this));
    newTransactionsView->hide();

    accountsView.reset(new views::Accounts(window.get(), this));
    accountsView->hide();

    transactionGroupsView.reset(new views::TransactionGroups(window.get(), this));
    transactionGroupsView->hide();

    matchConditionsView.reset(new views::MatchConditions(window.get(), this));
    matchConditionsView->hide();

    return app.exec();
}

void EconomyViewer::closeCurrentView(){
    switch(currentView){
    case ViewNames::TRANSACTIONS:
        transactionsView->hide();
        break;
    case ViewNames::NEW_TRANSACTIONS:
        newTransactionsView->hide();
        break;
    case ViewNames::MANAGE_ACCOUNTS:
        accountsView->hide();
        break;
    case ViewNames::TRANSACTIONS_GROUPS:
        transactionGroupsView->hide();
        break;
    case ViewNames::MATCH_CONDITIONS:
        matchConditionsView->hide();
        break;
    default:
        break;
    }
    currentView = ViewNames::NONE;
}

void EconomyViewer::openTransactionsViewAndAddTransactions(std::vector<Transaction>& transactions){
    closeCurrentView();
    transactionsView->openView(transactions, true);
    currentView = ViewNames::TRANSACTIONS;
}

void EconomyViewer::openTransactionsView(bool useCoreTransactionVector){
    closeCurrentView();
    if(useCoreTransactionVector)
        transactionsView->openView(_transactions);
    else
        transactionsView->openView();
    currentView = ViewNames::TRANSACTIONS;
}

void EconomyViewer::openNewTransactionsView(bool loadFileDialog){
    if(newTransactionsView->openView(_accounts, loadFileDialog)){
        closeCurrentView();
        currentView = ViewNames::NEW_TRANSACTIONS;
    }
}

void EconomyViewer::openAccountsView(bool useCoreAccountsVector){
    closeCurrentView();
    if(useCoreAccountsVector)
        accountsView->openView(this->_accounts);
    else
        accountsView->openView();
    currentView = ViewNames::MANAGE_ACCOUNTS;
}

void EconomyViewer::openTransactionGroupsView(bool useCoreTransactionGroupsVector){
    closeCurrentView();
    if(useCoreTransactionGroupsVector)
        transactionGroupsView->openView(_transactionGroups);
    else
        transactionGroupsView->openView();

    currentView = ViewNames::TRANSACTIONS_GROUPS;
}

void EconomyViewer::openTransactionGroupsView(std::map<unsigned int, core::TransactionGroup>& transactionGroups){
    closeCurrentView();
    transactionGroupsView->openView(transactionGroups, true);

    currentView = ViewNames::TRANSACTIONS_GROUPS;
}

void EconomyViewer::openMatchConditionsView(core::TransactionGroup& transactionGroup){
    closeCurrentView();
    matchConditionsView->openView(transactionGroup);
    currentView = ViewNames::MATCH_CONDITIONS;
}

void EconomyViewer::updateAccounts(std::vector<Account>& updatedAccounts){
    _accounts = updatedAccounts;
}

void EconomyViewer::updateTransactions(std::vector<Transaction>& updatedTransactions){
    _transactions.clear();
    for(Transaction& transaction: updatedTransactions){
        unsigned int id = transaction.getId();
        _transactions[id] = transaction;
    }
}

void EconomyViewer::updateTransactionGroups(std::map<unsigned int, core::TransactionGroup>& updatedTransactionGroups){
    _transactionGroups = updatedTransactionGroups;
}

unsigned int EconomyViewer::getUniqueTransactionId(){
    return _transactionId++;
}

unsigned int EconomyViewer::getUniqueTransactionGroupId(){
    return _transactionGroupId++;
}

TableRowState::TableRowState EconomyViewer::getTransactionTableRowState(core::Transaction& transaction){
    unsigned int id = transaction.getId();
    if(_transactions.count(id)){
        if(_transactions[id] == transaction)
            return TableRowState::UNCHANGED;
        else
            return TableRowState::CHANGED;
    }
    return TableRowState::NEW;
}

TableRowState::TableRowState EconomyViewer::getTransactionGroupTableRowState(core::TransactionGroup& transactionGroup){
    unsigned int id = transactionGroup.id();
    if(_transactionGroups.count(id)){
        if(_transactionGroups[id] == transactionGroup)
            return TableRowState::UNCHANGED;
        else
            return TableRowState::CHANGED;
    }
    return TableRowState::NEW;
}

bool EconomyViewer::saveToFile(std::string fileName){
    std::ofstream file(fileName);
    if(!file.is_open())
        return false;

    JsonNode node, accounts, transactions, transactionGroups;
    accounts.type(JsonNodeType::ARRAY);
    transactions.type(JsonNodeType::ARRAY);
    transactionGroups.type(JsonNodeType::ARRAY);

    for(auto& account: _accounts)
        accounts.addToArray(account.toJson());
    for(auto& [key, val]: _transactions)
        transactions.addToArray(val.toJson());
    for(auto& [key, val]: _transactionGroups)
        transactionGroups.addToArray(val.toJson());

    node.addToObject("_accounts", accounts);
    node.addToObject("_transactions", transactions);
    node.addToObject("_transactionGroups", transactionGroups);
    node.addToObject("_transactionId", _transactionId);
    node.addToObject("_transactionGroupId", _transactionGroupId);
    node.addToObject("_fileVersion", "1.0");
    file<<node.getJson();
    file.close();
    return !file.fail() && !node.error();
}

bool EconomyViewer::loadFromFile(std::string fileName){
    std::ifstream file(fileName);
    if(!file.is_open())
        return false;

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    JsonNode json(buffer.str());
    return fromJson(json) && !json.error() && !file.fail();
}

bool EconomyViewer::fromJson(JsonNode node){
    if(node.type() != JsonNodeType::OBJECT)
        return false;
    clear();
    auto& obj = node.objectRef();
    if(obj.count("_accounts")){
        auto accountsNode = *obj["_accounts"];
        if(accountsNode.type() != JsonNodeType::ARRAY){
        }else{
            auto& accounts = accountsNode.arrayRef();
            for(auto& accountNodePtr: accounts){
                Account account;
                if(account.fromJson(*accountNodePtr))
                    _accounts.push_back(account);
            }
        }
    }
    if(obj.count("_transactions")){
        auto transactionsNode = *obj["_transactions"];
        if(transactionsNode.type() != JsonNodeType::ARRAY){
        }else{
            auto& transactions = transactionsNode.arrayRef();
            for(auto& transactionNodePtr: transactions){
                Transaction transaction;
                if(transaction.fromJson(*transactionNodePtr))
                    _transactions[transaction.getId()] = transaction;
            }
        }
    }
    if(obj.count("_transactionGroups")){
        auto transactionGroupsNode = *obj["_transactionGroups"];
        if(transactionGroupsNode.type() != JsonNodeType::ARRAY){
        }else{
            auto& transactionGroups = transactionGroupsNode.arrayRef();
            for(auto& transactionGroupNodePtr: transactionGroups){
                TransactionGroup transactionGroup;
                if(transactionGroup.fromJson(*transactionGroupNodePtr))
                    _transactionGroups[transactionGroup.id()] = transactionGroup;
            }
        }
    }
    if(obj.count("_transactionId")){
        auto transactionId = *obj["_transactionId"];
        if(transactionId.type() != JsonNodeType::VALUE)
            return false;
        _transactionId = transactionId.valueAsInt();
    }else
        return false;
    if(obj.count("_transactionGroupId")){
        auto transactionGroupId = *obj["_transactionGroupId"];
        if(transactionGroupId.type() != JsonNodeType::VALUE)
            return false;
        _transactionGroupId = transactionGroupId.valueAsInt();
    }else
        return false;
    return true;
}

void EconomyViewer::clear(){
    _accounts.clear();
    _transactions.clear();
    _transactionGroups.clear();
    _transactionId=1;
    _transactionGroupId=1;
}

}
