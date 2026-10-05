#include <archimedes/scene/InvalidEntityException.h>

namespace arch::scene {

InvalidEntityException::InvalidEntityException(std::string_view msg, const utils::SimpleSourceLocation& loc) noexcept:
	Exception("InvalidEntityException", msg.data(), loc) {}

} // namespace arch::scene
