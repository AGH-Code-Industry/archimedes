#pragma once

#include "KeyState.h"

#define STATE(x)                        \
	constexpr bool x() const noexcept { \
		return value & *KeyState::x;    \
	}

namespace arch::input {

struct KeyStateWrapper {
	const u32 value = *KeyState::up;

	constexpr bool operator>(const u32 state) const noexcept { return (value & state) == state; }

	STATE(up)
	STATE(released)
	STATE(down)
	STATE(pressed)
	STATE(repeat)

	STATE(shift)
	STATE(ctrl)
	STATE(control)
	STATE(alt)
	STATE(super)
	STATE(win)
	STATE(windows)
	STATE(command)
	STATE(meta)
	STATE(capsLock)
	STATE(numLock)
};

constexpr bool operator<(const u32 lhs, const KeyStateWrapper rhs) noexcept {
	return rhs > lhs;
}

} // namespace arch::input

#undef STATE
