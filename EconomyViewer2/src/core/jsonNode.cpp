#include "../../include/core/jsonNode.h"

namespace core {


JsonNodeType::JsonNodeType JsonDecoder::getType(){
    skipWhitespace();
    if(_error)
        return JsonNodeType::OBJECT;    // Its not true, but its something
    char c = _json[_pos];
    if(c == '{')
        return JsonNodeType::OBJECT;
    if(c == '[')
        return JsonNodeType::ARRAY;
    if(c == '\"')
        return JsonNodeType::VALUE;
    _error = true;
    return JsonNodeType::OBJECT;         // Its not true, but its something
}

std::string JsonDecoder::getValue(){
    if(!skipWhitespace())
        return "";
    if(_json[_pos] == '\"')
        return getString();
    if(_json[_pos] == '[')
        return getArray();
    if(_json[_pos] == '{')
        return getObject();
    _error = true;
    return "";
}

char JsonDecoder::currChar(){
    if(checkEos())
        return ' ';                         // Blank space is as close to nothing I can think of
    return _json[_pos];
}

bool JsonDecoder::getColon(){
    skipWhitespace();
    if(checkEos())
        return false;
    if(currChar() == ':'){
        _pos++;
        return true;
    }
    return false;
}

bool JsonDecoder::moreElements(){
    if(!skipWhitespace())
        return false;
    if(currChar() == ','){
        _pos++;
        return true;
    }
    return false;
}

bool JsonDecoder::skipWhitespace(){
    if(checkEos())
        return false;
    while(_json[_pos]==' ' || _json[_pos]=='\r' || _json[_pos]=='\n' || _json[_pos]=='\t'){
        _pos++;
        if(checkEos())
            return false;
    }
    return true;
}

bool JsonDecoder::gotoEndOfString(){
    return gotoEndChar('\"');
}

bool JsonDecoder::gotoEndOfObject(){
    return gotoEndChar('}');
}

bool JsonDecoder::gotoEndOfArray(){
    return gotoEndChar(']');
}

bool JsonDecoder::gotoEndChar(char c){
    int curly = 0;
    int square = 0;
    if(c=='}')
        curly++;
    if(c==']')
        square++;
    while(nextChar()){
        if(_json[_pos] == '{')
            curly++;
        if(_json[_pos] == '}')
            curly--;
        if(_json[_pos] == '[')
            square++;
        if(_json[_pos] == ']')
            square--;
        if(_json[_pos] == c && curly == 0 && square == 0)
            return true;
    }
    return false;
}


bool JsonDecoder::nextChar(){
    _pos++;
    if(checkEos())
        return false;
    while(_json[_pos] == '\\'){
        _pos++;
        if(checkEos())
            return false;
        if(_json[_pos] == 'u')
            _pos += 4;
        else
            _pos++;
        if(checkEos())
            return false;
    }
    return true;
}

bool JsonDecoder::checkEos(){
    if(_pos >= (int)_json.length()){
        _error = true;
        return true;
    }
    return false;
}

std::string JsonDecoder::getString(){
    skipWhitespace();
    if(_error)
        return "";
    if(_json[_pos] != '\"'){
        _error = true;
        return "";
    }
    int startPos = _pos;
    gotoEndOfString();
    if(_error)
        return "";
    _pos++;
    return _json.substr(startPos, _pos-startPos);
}

std::string JsonDecoder::getStringContents(){
    std::string str = getString();
    if(_error)
        return "";
    return str.substr(1, str.length()-2);
}

std::string JsonDecoder::getArray(){
    skipWhitespace();
    if(_error)
        return "";
    if(_json[_pos] != '['){
        _error = true;
        return "";
    }
    int startPos = _pos;
    gotoEndOfArray();
    if(_error)
        return "";
    _pos++;
    return _json.substr(startPos, _pos-startPos);
}

std::string JsonDecoder::getObject(){
    skipWhitespace();
    if(_error)
        return "";
    if(_json[_pos] != '{'){
        _error = true;
        return "";
    }
    int startPos = _pos;
    gotoEndOfObject();
    if(_error)
        return "";
    _pos++;
    return _json.substr(startPos, _pos-startPos);
}

void JsonNode::setFromString(std::string json){
    JsonDecoder dec(json);
    _type = dec.getType();
    if(dec.error()){
        _error = true;
        return;
    }
    bool first = true;
    if(_type == JsonNodeType::OBJECT){
        dec.nextChar();
        if(!dec.skipWhitespace()){
            _error = true;
            return;
        }
        while((first && dec.currChar() != '}') || dec.moreElements()){
            first = false;
            std::string name = dec.getStringContents();
            dec.getColon();
            std::string value = dec.getValue();
            if(dec.error())
                _error = true;
            else{
                auto nodePtr = std::make_shared<JsonNode>(value);
                if(nodePtr->error())
                    _error = true;
                else
                    _object[name] = nodePtr;
            }
        }
        if(dec.currChar() != '}')
            _error = true;
    }else if(_type == JsonNodeType::ARRAY){
        dec.nextChar();
        if(!dec.skipWhitespace()){
            _error = true;
            return;
        }
        while((first && dec.currChar() != ']') || dec.moreElements()){
            first = false;
            std::string value = dec.getValue();
            if(dec.error())
                _error = true;
            else{
                auto nodePtr = std::make_shared<JsonNode>(value);
                if(nodePtr->error())
                    _error = true;
                else
                    _array.push_back(nodePtr);
            }
        }
        if(dec.currChar() != ']')
            _error = true;
    }else if(_type == JsonNodeType::VALUE){
        _value = dec.getStringContents();
        if(dec.error())
            _error = true;
    }
}

std::string JsonNode::getJson(){
    std::string ret = "";
    bool first = true;
    switch(_type){
    case JsonNodeType::OBJECT:
        ret += '{';
        for(auto& [key, val]: _object){
            if(!first)
                ret += ",";
            first = false;
            ret += "\"" + key + "\":";
            ret += val->getJson();
        }
        ret += '}';
        break;
    case JsonNodeType::ARRAY:
        ret += '[';
        for(auto& val: _array){
            if(!first)
                ret += ",";
            first = false;
            ret += val->getJson();
        }
        ret += ']';
        break;
    case JsonNodeType::VALUE:
        ret += "\"" + _value + "\"";
        break;
    default:
        break;
    }
    return ret;
}

std::string JsonNode::encodeStr(std::string str){
    std::string ret = "";
    for(char& c: str){
        switch(c){
        case '\"':
            ret += "\\\"";
            break;
        case '\\':
            ret += "\\\\";
            break;
        case '/':
            ret += "\\/";
            break;
        case '\b':
            ret += "\\\b";
            break;
        case '\f':
            ret += "\\\f";
            break;
        case '\n':
            ret += "\\\n";
            break;
        case '\r':
            ret += "\\\r";
            break;
        case '\t':
            ret += "\\\t";
            break;
        // Case \uxxxx is not supported, not sure how I will know if a char is a unicode and not normal json supported char
        default:
            ret += c;
            break;
        }
    }
    return ret;
}

std::string JsonNode::decodeStr(std::string str){
    std::string ret = "";
    for(int i=0; i<(int)str.length(); i++){
        if(str[i] == '\\'){
            i++;
            if(i<(int)str.length()){
                char c = str[i];
                switch(c){
                case '\"':
                    ret += "\"";
                    break;
                case '\\':
                    ret += "\\";
                    break;
                case '/':
                    ret += "/";
                    break;
                case '\b':
                    ret += "\b";
                    break;
                case '\f':
                    ret += "\f";
                    break;
                case '\n':
                    ret += "\n";
                    break;
                case '\r':
                    ret += "\r";
                    break;
                case '\t':
                    ret += "\t";
                    break;
                    // Case \uxxxx is not supported, not sure how to add any unicode char to a c++ string (didnt bother googeling it)
                default:
                    ret += c;
                }
            }else{
                // lonely \ in the end of string, not valid JSON. Lets just throw it away
            }
        }
    }
    return ret;
}

}
