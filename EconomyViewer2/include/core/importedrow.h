#ifndef IMPORTEDROW_H
#define IMPORTEDROW_H

#include <vector>
#include <string>
#include "../core/constants.h"

namespace core{

class ImportedRow
{
public:
    ImportedRow();
    ImportedRow(std::string row);
    ImportedRow(const ImportedRow& other);
    ImportedRow& operator=(const ImportedRow& other);
    void parseRow(std::string row);
    bool isValid() {return _validRow;}
    std::vector<std::string> columns() {return _columns;}
    std::vector<int> dateColumns() {return _dateColumns;}
    std::vector<int> numColumns() {return _numColumns;}
    std::vector<int> textColumns() {return _textColumns;}
    bool isColumnDate(int columnId);
    bool isColumnNum(int columnId);
    bool isColumnText(int columnId);
    std::tm getColumnAsDate(int columnId);
    int getColumnAsNum(int columnId);
    std::string getColumnAsText(int columnId);

private:
    bool _validRow;
    std::vector<std::string> _columns;
    std::vector<int> _dateColumns;
    std::vector<int> _numColumns;
    std::vector<int> _textColumns;
};

}

#endif // IMPORTEDROW_H
