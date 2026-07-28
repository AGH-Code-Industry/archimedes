#pragma once

#include <string>

#include <archimedes/Exception.h>

namespace arch {

class ConfigException final: public Exception {
public:
	ConfigException(const std::string& message, const std::stacktrace& stacktrace = std::stacktrace::current());
};

} // namespace arch
