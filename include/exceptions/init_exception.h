#pragma once

#include "exception.h"

namespace arch {

    class InitException : public Exception {
    public:
        InitException();
        InitException(const std::string description);
    };
}

