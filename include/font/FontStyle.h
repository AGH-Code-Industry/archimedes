#pragma once

namespace arch::font {

enum FontStyle : unsigned int {
	regular = 1 << 0,
	italic = 1 << 1,
	bold = 1 << 2
};

} // namespace arch::font
