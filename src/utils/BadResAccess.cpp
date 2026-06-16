#include <archimedes/utils/res/BadResAccess.h>

arch::utils::BadResAccess::BadResAccess(std::string_view msg, const std::source_location& location) noexcept:
	Exception("BadResAccess", msg.data(), location) {}
