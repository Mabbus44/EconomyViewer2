#include "../../include/core/importedrow.h"
#include "../../include/core/constants.h"

namespace core{

ImportedRow::ImportedRow() {}

ImportedRow::ImportedRow(std::string row){
    parseRow(row);
}

ImportedRow::ImportedRow(const ImportedRow& other){
    _validRow = other._validRow;
    _columns = other._columns;
    _dateColumns = other._dateColumns;
    _numColumns = other._numColumns;
    _textColumns = other._textColumns;
}

ImportedRow& ImportedRow::operator=(const ImportedRow& other){
    _validRow = other._validRow;
    _columns = other._columns;
    _dateColumns = other._dateColumns;
    _numColumns = other._numColumns;
    _textColumns = other._textColumns;
    return *this;
}

void ImportedRow::parseRow(std::string row){
    _columns = core::Utils::splitString(row, ";");
    for(int col = 0; col < (int)_columns.size(); col++){
        if(Utils::isDate(_columns[col])){
            _dateColumns.push_back(col);
        }
        else if(Utils::isNum(_columns[col]))
            _numColumns.push_back(col);
        else if(_columns[col].size() > 0){
            _textColumns.push_back(col);
        }
    }
    _validRow = (_dateColumns.size() > 0 && _numColumns.size() > 0);
}

bool ImportedRow::isColumnDate(int columnId){
    if(columnId >= (int)_columns.size())
        return false;
    return Utils::isDate(_columns[columnId]);
}

bool ImportedRow::isColumnNum(int columnId){
    if(columnId >= (int)_columns.size())
        return false;
    return Utils::isNum(_columns[columnId]);
}

bool ImportedRow::isColumnText(int columnId){
    return columnId < (int)_columns.size();
    // return !isNum(_columns[columnId]) && !isDate(_columns[columnId]); // I probably should allow texts that are nums dates
}

std::tm ImportedRow::getColumnAsDate(int columnId){
    if(!isColumnDate(columnId)){
        core::Utils::showErrorMessage("Error: Tried to access illigal column as date!");
        std::tm ret;
        return ret;
    }
    return Utils::toDate(_columns[columnId]);
}

int ImportedRow::getColumnAsNum(int columnId){
    if(!isColumnNum(columnId)){
        core::Utils::showErrorMessage("Error: Tried to access illigal column as num!");
        return 0;
    }
    return Utils::toInt(_columns[columnId]);
}

std::string ImportedRow::getColumnAsText(int columnId){
    if(!isColumnText(columnId)){
        core::Utils::showErrorMessage("Error: Tried to access illigal column as text!");
        return "";
    }
    return _columns[columnId];
}

}
