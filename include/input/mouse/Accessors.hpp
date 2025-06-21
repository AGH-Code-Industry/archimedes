#include "Accessors.h"

#define ARCH_ACCESSOR(x)                   \
	inline KeyStateWrapper x() noexcept {  \
		return _details::state[Button::x]; \
	}

namespace arch::input::mouse {

inline KeyStateWrapper button(const u32 buttonCode) noexcept {
	return _details::state[buttonCode];
}

inline double x() noexcept {
	return _details::state.x();
}

inline double y() noexcept {
	return _details::state.y();
}

inline double2 pos() noexcept {
	return { x(), y() };
}

inline double dx() noexcept {
	return _details::state.dx();
}

inline double dy() noexcept {
	return _details::state.dy();
}

inline double2 dpos() noexcept {
	return { dx(), dy() };
}

namespace scroll {

inline double x() noexcept {
	return _details::state.scrollX();
}

inline double y() noexcept {
	return _details::state.scrollY();
}

} // namespace scroll

namespace cursor {

inline void disabled(bool value, bool setRaw) noexcept {
	_details::state.cursorDisabled(value);
	rawInput(value && setRaw);
}

inline bool disabled() noexcept {
	return _details::state.cursorDisabled();
}

inline void toggle(bool toggleRaw) noexcept {
	disabled(!disabled(), toggleRaw);
}

} // namespace cursor

inline void rawInput(bool value) noexcept {
	_details::state.raw(value);
}

inline void rawInputToggle() noexcept {
	rawInput(!rawInput());
}

inline bool rawInput() noexcept {
	return _details::state.raw();
}

inline bool supportsRaw() noexcept {
	return _details::state.supportsRaw();
}

ARCH_ACCESSOR(left)
ARCH_ACCESSOR(right)
ARCH_ACCESSOR(middle)
ARCH_ACCESSOR(first)
ARCH_ACCESSOR(second)
ARCH_ACCESSOR(third)
ARCH_ACCESSOR(fourth)
ARCH_ACCESSOR(fifth)
ARCH_ACCESSOR(sixth)
ARCH_ACCESSOR(seventh)
ARCH_ACCESSOR(eighth)

} // namespace arch::input::mouse

#undef ARCH_ACCESSOR
