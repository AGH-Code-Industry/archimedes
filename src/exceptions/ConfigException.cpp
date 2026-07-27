#include <archimedes/exceptions/ConfigException.h>

namespace arch {

ConfigException::ConfigException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("Config", message, stacktrace) {}

} // namespace arch
