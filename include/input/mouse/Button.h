#pragma once

#include <Mmath.h>

namespace arch::input::mouse {

struct Button {
	enum Code : u32 {
		left,
		right,
		middle,

		first = left,
		second,
		third,
		fourth,
		fifth,
		sixth,
		seventh,
		eighth,
	};
};

} // namespace arch::input::mouse
