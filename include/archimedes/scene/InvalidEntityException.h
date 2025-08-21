#pragma once

#include <Exception.h>

namespace arch::scene {

class InvalidEntityException: public arch::Exception {
public:
	InvalidEntityException(
		std::string_view msg,
		const std::source_location& location = std::source_location::current()
	) noexcept;
};

} // namespace arch::scene
