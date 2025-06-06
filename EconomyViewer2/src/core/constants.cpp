#include "../../include/core/constants.h"

namespace core{

void showErrorMessage(std::string text){
    QMessageBox messageBox;
    messageBox.critical(0,"Error", text.c_str());
    messageBox.setFixedSize(500,200);
};

}
