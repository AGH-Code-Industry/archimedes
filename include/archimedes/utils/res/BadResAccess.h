#pragma once

#include <Exception.h>

namespace arch::utils {

class BadResAccess: public arch::Exception {
public:
	BadResAccess(std::string_view msg, const std::source_location& location = std::source_location::current()) noexcept;
};

} // namespace arch::utils
