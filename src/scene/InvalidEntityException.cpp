#include <scene/InvalidEntityException.h>

namespace arch::scene {

InvalidEntityException::InvalidEntityException(std::string_view msg, const std::source_location& location) noexcept:
	Exception("InvalidEntityException", msg.data(), location) {}

} // namespace arch::scene
