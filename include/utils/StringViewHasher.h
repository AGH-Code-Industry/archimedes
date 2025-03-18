#pragma once

#include <string_view>

namespace arch::utils {

struct StringViewHasher: public std::hash<std::string_view> {
	using is_transparent = void;
};

} // namespace arch::utils
