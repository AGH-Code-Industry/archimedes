#include "graphics/glad_exception.h"

namespace arch {

GladException::GladException() : Exception("GLAD") {
	append_msg("Failed to initialize GLAD");
}

}