#include <archimedes/utils/res/BadResAccess.h>

arch::utils::BadResAccess::BadResAccess(std::string_view msg, const std::stacktrace& stacktrace) noexcept:
	Exception("BadResAccess", msg.data(), stacktrace) {}
