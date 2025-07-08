#include <Window.h>
#include <input/Key.h>
#include <input/Mouse.h>

namespace arch::input {

#define ARCH_BUTTON(_button) Key Mouse::_button{};

ARCH_BUTTON(first)
ARCH_BUTTON(second)
ARCH_BUTTON(third)
ARCH_BUTTON(fourth)
ARCH_BUTTON(fifth)
ARCH_BUTTON(sixth)
ARCH_BUTTON(seventh)
ARCH_BUTTON(eighth)

#undef ARCH_BUTTON

Ref<Window> Mouse::_window;
double Mouse::_x;
double Mouse::_y;
double Mouse::_dx;
double Mouse::_dy;
double Mouse::_scrollx;
double Mouse::_scrolly;
bool Mouse::_delta;
bool Mouse::_cursorDisabled;
bool Mouse::_rawInput;

void Mouse::_init(const Ref<Window>& window) noexcept {
	_window = window;
}

void Mouse::Cursor::disabled(bool value, bool setRaw) noexcept {
	if (Mouse::_window) {
		glfwSetInputMode(Mouse::_window->get(), GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		Mouse::_cursorDisabled = value;
		if (!value) {
			Mouse::_rawInput = false;
			Mouse::_delta = false;
		}
	}
	Mouse::RawInput::enabled(value && setRaw);
}

void Mouse::RawInput::enabled(bool value) noexcept {
	if (supported()) {
		if (Mouse::_window && Mouse::_cursorDisabled) {
			glfwSetInputMode(Mouse::_window->get(), GLFW_RAW_MOUSE_MOTION, value ? GLFW_TRUE : GLFW_FALSE);
			Mouse::_rawInput = value;
		}
	}
}

bool Mouse::RawInput::supported() noexcept {
	return glfwRawMouseMotionSupported();
}

} // namespace arch::input

