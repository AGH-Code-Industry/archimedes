#include "KeyStateWrapper.h"

#define ARCH_STATE(x)                                    \
	constexpr bool KeyStateWrapper::x() const noexcept { \
		return value & *KeyState::x;                     \
	}

namespace arch::input {

constexpr KeyStateWrapper::Time KeyStateWrapper::upTime() const noexcept {
	return time * up();
}

constexpr KeyStateWrapper::Time KeyStateWrapper::downTime() const noexcept {
	return time * down();
}

ARCH_STATE(up)
ARCH_STATE(released)
ARCH_STATE(down)
ARCH_STATE(pressed)
ARCH_STATE(repeat)
ARCH_STATE(shift)
ARCH_STATE(ctrl)
ARCH_STATE(control)
ARCH_STATE(alt)
ARCH_STATE(super)
ARCH_STATE(win)
ARCH_STATE(windows)
ARCH_STATE(command)
ARCH_STATE(meta)
ARCH_STATE(capsLock)
ARCH_STATE(numLock)

} // namespace arch::input

#undef ARCH_STATE
