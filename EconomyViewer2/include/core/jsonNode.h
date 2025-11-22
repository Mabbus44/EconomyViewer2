#ifndef JSONNODE_H
#define JSONNODE_H
#include <string>
#include <map>
#include <vector>
#include <memory>
#include "../core/constants.h"

namespace JsonNodeType{ enum JsonNodeType{
    OBJECT,
    ARRAY,
    VALUE
};}


namespace core{

class JsonDecoder // Nasty class that trust that you call the methods in the correct order. If you dont it will assume the json is wrong.
{
public:
    JsonDecoder(){_json="";};
    JsonDecoder(std::string json){_json = json;}
    JsonNodeType::JsonNodeType getType();
    std::string getString();
    std::string getStringContents();
    std::string getValue();
    char currChar();
    bool getColon();
    bool moreElements();
    bool error() {return _error;}
    bool skipWhitespace();
    bool nextChar();
private:
    bool gotoEndOfString();
    bool gotoEndOfObject();
    bool gotoEndOfArray();
    bool gotoEndChar(char c);
    bool checkEos();
    std::string getObject();
    std::string getArray();
    std::string _json;
    int _pos = 0;
    bool _error = false;
};

class JsonNode
{
public:
    JsonNode(){};
    JsonNode(std::string json){setFromString(json);};
    void addToObject(std::string key, JsonNode value){_object[key] = std::make_shared<JsonNode>(value); _type = JsonNodeType::OBJECT;}
    void addToObject(std::string key, std::string value){_object[key] = std::make_shared<JsonNode>("\"" + encodeStr(value) + "\""); _type = JsonNodeType::OBJECT;}
    void addToObject(std::string key, int value){_object[key] = std::make_shared<JsonNode>("\"" + encodeStr(std::to_string(value)) + "\""); _type = JsonNodeType::OBJECT;}
    void addBoolToObject(std::string key, bool value){_object[key] = std::make_shared<JsonNode>("\"" + encodeStr(Utils::toString(value)) + "\""); _type = JsonNodeType::OBJECT;}
    void addJsonStringToObject(std::string key, std::string value){_object[key] = std::make_shared<JsonNode>(value); _type = JsonNodeType::OBJECT;}
    void addToArray(JsonNode value) {_array.push_back(std::make_shared<JsonNode>(value)); _type = JsonNodeType::ARRAY;}
    void addToArray(std::string value) {_array.push_back(std::make_shared<JsonNode>("\"" + encodeStr(value) + "\"")); _type = JsonNodeType::ARRAY;}
    void addToArray(int value) {_array.push_back(std::make_shared<JsonNode>("\"" + encodeStr(std::to_string(value)) + "\"")); _type = JsonNodeType::ARRAY;}
    void addJsonStringToArray(std::string value) {_array.push_back(std::make_shared<JsonNode>(value)); _type = JsonNodeType::ARRAY;}
    void addBoolToArray(bool value) {_array.push_back(std::make_shared<JsonNode>("\"" + encodeStr(Utils::toString(value)) + "\"")); _type = JsonNodeType::ARRAY;}
    void value(std::string value) {_value = value; _type = JsonNodeType::VALUE;}
    void type(JsonNodeType::JsonNodeType type) {_type = type;}
    void setFromString(std::string json);
    std::string getJson();
    JsonNodeType::JsonNodeType type() {return _type;}
    std::string value() {return _value;}
    int valueAsInt() {return Utils::toInt(_value);}
    int valueAsBool() {return Utils::toBool(_value);}
    std::map<std::string, std::shared_ptr<JsonNode>>& objectRef() {return _object;}
    std::vector<std::shared_ptr<JsonNode>>& arrayRef() {return _array;}
    bool error() {return _error;}
private:
    static std::string encodeStr(std::string str);
    static std::string decodeStr(std::string str);
    std::map<std::string, std::shared_ptr<JsonNode>> _object;
    std::vector<std::shared_ptr<JsonNode>> _array;
    std::string _value;
    JsonNodeType::JsonNodeType _type;
    bool _error = false;
};

}

#endif // JSONNODE_H
