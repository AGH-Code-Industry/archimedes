#include "exceptions/config_exception.h"

namespace arch {
    ConfigException::ConfigException() : Exception("CONFIG", "Failed while loading config.") {}

    ConfigException::ConfigException(const std::string description) : Exception("CONFIG") {
        set_description(description);
    }
}
