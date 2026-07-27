#pragma once

#include <archimedes/Exception.h>

namespace arch::scene {

class InvalidEntityException: public arch::Exception {
public:
	InvalidEntityException(
		std::string_view msg,
		const std::stacktrace& stacktrace = std::stacktrace::current()
	) noexcept;
};

} // namespace arch::scene
