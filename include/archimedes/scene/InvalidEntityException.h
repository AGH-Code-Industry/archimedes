#pragma once

#include <archimedes/Exception.h>

namespace arch::scene {

class InvalidEntityException: public arch::Exception {
public:
	InvalidEntityException(
		std::string_view msg,
		const utils::SimpleSourceLocation& loc = utils::SimpleSourceLocation::current()
	) noexcept;
};

} // namespace arch::scene
