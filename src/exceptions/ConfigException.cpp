#include <archimedes/exceptions/ConfigException.h>

namespace arch {

ConfigException::ConfigException(const std::string& message, const std::source_location& location):
	Exception("Config", message, location) {}

} // namespace arch
