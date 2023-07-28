//
// Created by tosiek on 24.02.23.
//

#include "exceptions/config_exception.h"

using namespace arch;

ConfigException::ConfigException(const std::string& message) {
    _message = "Failed load configuration file: " + message;
}

const char *ConfigException::what() const noexcept {
    return _message.c_str();
}
