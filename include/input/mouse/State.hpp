#include "State.h"

namespace arch::input::mouse::_details {

KeyStateWrapper State::operator[](const u32 buttonCode) noexcept {
	return KeyStateWrapper{ .value = _state[buttonCode], .time = _time[buttonCode] };
}

inline double State::x() const noexcept {
	return _x;
}

inline double State::y() const noexcept {
	return _y;
}

inline double State::dx() const noexcept {
	return _dx;
}

inline double State::dy() const noexcept {
	return _dy;
}

inline double State::scrollX() const noexcept {
	return _xscroll;
}

inline double State::scrollY() const noexcept {
	return _yscroll;
}

inline void State::cursorDisabled(bool value) noexcept {
	if (_window) {
		glfwSetInputMode(_window->get(), GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		_cursorDisabled = value;
		if (!value) {
			_raw = false;
			_delta = false;
		}
	}
}

inline bool State::cursorDisabled() const noexcept {
	return _cursorDisabled;
}

inline void State::raw(bool value) noexcept {
	if (supportsRaw()) {
		if (_window && _cursorDisabled) {
			glfwSetInputMode(_window->get(), GLFW_RAW_MOUSE_MOTION, value ? GLFW_TRUE : GLFW_FALSE);
			_raw = value;
		}
	}
}

inline bool State::raw() const noexcept {
	return _raw;
}

inline bool State::supportsRaw() const noexcept {
	return glfwRawMouseMotionSupported();
}

} // namespace arch::input::mouse::_details
