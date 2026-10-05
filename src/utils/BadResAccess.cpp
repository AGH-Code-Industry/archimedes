#include <archimedes/utils/res/BadResAccess.h>

arch::utils::BadResAccess::BadResAccess(std::string_view msg, const utils::SimpleSourceLocation& loc) noexcept:
	Exception("BadResAccess", msg.data(), loc) {}
