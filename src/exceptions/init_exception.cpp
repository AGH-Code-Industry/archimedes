//
// Created by tosiek on 22.02.23.
//

#include <exceptions/init_exception.h>

using namespace arch;

InitException::InitException(const std::string& message) {
    _message = "Failed while initialization: " + message;
}

const char *InitException::what() const noexcept {
    return _message.c_str();
}