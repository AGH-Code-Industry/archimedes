#include "graphics/glad_exception.h"

namespace arch {

GladException::GladException() : Exception("GLAD", "Failed to initialize GLAD.") {}

GladException::GladException(const std::string description) : Exception("GLAD") {
    set_description(description);
}
}