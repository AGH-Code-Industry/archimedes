#pragma once

#include <string>

#include <archimedes/Exception.h>

namespace arch {

class ConfigException final: public Exception {
public:
	ConfigException(
		const std::string& message,
		const utils::SimpleSourceLocation& loc = utils::SimpleSourceLocation::current()
	);
};

} // namespace arch
