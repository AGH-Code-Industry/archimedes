#include <archimedes/scene/InvalidEntityException.h>

namespace arch::scene {

InvalidEntityException::InvalidEntityException(std::string_view msg, const std::stacktrace& stacktrace) noexcept:
	Exception("InvalidEntityException", msg.data(), stacktrace) {}

} // namespace arch::scene
