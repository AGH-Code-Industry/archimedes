#include "exceptions/init_exception.h"

namespace arch {

    InitException::InitException() : Exception("INITIALIZATION", "Something failed during initialization.") {}

    InitException::InitException(const std::string description) : Exception("INITIALIZATION") {
        set_description(description);
    }
}

