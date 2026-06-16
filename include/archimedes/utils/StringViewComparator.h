#pragma once

#include <string_view>

namespace arch::utils {

struct StringViewComparator: public std::equal_to<std::string_view> {
	using is_transparent = void;
};

} // namespace arch::utils
