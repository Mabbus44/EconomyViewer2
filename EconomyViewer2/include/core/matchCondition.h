#ifndef MATCHCONDITION_H
#define MATCHCONDITION_H

#include "../core/constants.h"
#include "../core/transaction.h"
#include "../../include/core/jsonNode.h"

namespace core{

    class MatchCondition
    {
    public:
        MatchCondition(){}
        ~MatchCondition(){}
        bool operator==(const MatchCondition& other) const;

        bool isMatch(Transaction& transaction);

        void compareType(CompareType::CompareType c){_compareType = c;}
        void compareProperty(CompareProperty::CompareProperty c){_compareProperty = c;}
        void valueDate(std::tm v){_valueDate = v;}
        void valueString(std::string v){_valueString = v;}
        void valueInt(int v){_valueInt = v;}
        void id(unsigned int id) {_id=id;}
        void setCompareTypeWithString(std::string compareType){_compareType = stringToCompareType(compareType);}
        void setComparePropertyWithString(std::string compareType){_compareType = stringToCompareType(compareType);}
        void setValueWithString(std::string value);

        CompareType::CompareType compareType() const {return _compareType;}
        CompareProperty::CompareProperty compareProperty() const {return _compareProperty;}
        std::string getValueAsString() const;
        std::string getCompareTypeString() const {return compareTypeToString(_compareType);}
        std::string getComparePropertyString() const {return comparePropertyToString(_compareProperty);}
        unsigned int id() const {return _id;}
        bool fromJson(JsonNode node);
        JsonNode toJson();
        void clear() {};

        static std::string compareTypeToString(CompareType::CompareType compareType);
        static std::string comparePropertyToString(CompareProperty::CompareProperty compareProperty);
        static CompareType::CompareType stringToCompareType(std::string compareType);
        static CompareProperty::CompareProperty stringToCompareProperty(std::string compareProperty);
    private:
        bool isDateMatch(std::tm date);
        bool isStringMatch(std::string str);
        bool isIntMatch(int num);

        CompareType::CompareType _compareType;
        CompareProperty::CompareProperty _compareProperty;
        std::tm _valueDate;
        std::string _valueString;
        int _valueInt;
        unsigned int _id;
    };
}

#endif // MATCHCONDITION_H
