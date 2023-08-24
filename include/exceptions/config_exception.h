#pragma once

#include "exception.h"

namespace arch {

    class ConfigException : public Exception {
    public:
        ConfigException();
        ConfigException(const std::string description);
    };
}

