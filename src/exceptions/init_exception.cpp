//
// Created by tosiek on 22.02.23.
//

#include "exceptions/init_exception.h"

using namespace arch;

const char* InitException::what() {
    if (_message == "") {
        return "Failed while initialization";
    }
    return "Failed while initialization: " + _message;
}