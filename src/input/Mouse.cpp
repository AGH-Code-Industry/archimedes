#include <archimedes/Window.h>
#include <archimedes/input/Key.h>
#include <archimedes/input/Mouse.h>

namespace arch::input {

#define ARCH_BUTTON(_button) Key Mouse::_button{};

ARCH_BUTTON(first)
ARCH_BUTTON(second)
Mouse::Scroll Mouse::scroll{};
ARCH_BUTTON(fourth)
ARCH_BUTTON(fifth)
ARCH_BUTTON(sixth)
ARCH_BUTTON(seventh)
ARCH_BUTTON(eighth)

#undef ARCH_BUTTON

Ref<Window> Mouse::_window{};
double Mouse::_x{};
double Mouse::_y{};
double Mouse::_dx{};
double Mouse::_dy{};
bool Mouse::_delta{};
Mouse::Cursor Mouse::cursor{};
Mouse::RawInput Mouse::rawInput{};

void Mouse::_setWindow(const Ref<Window>& window) noexcept {
	_window = window;
}

void Mouse::Cursor::disabled(bool value, bool setRaw) noexcept {
	if (Mouse::_window) {
		glfwSetInputMode(Mouse::_window->get(), GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		Mouse::cursor._disabled = value;
		if (!value) {
			Mouse::rawInput._enabled = false;
			Mouse::_delta = false;
		}
	}
	Mouse::rawInput.enabled(value && setRaw);
}

void Mouse::RawInput::enabled(bool value) noexcept {
	if (supported()) {
		if (Mouse::_window && Mouse::cursor._disabled) {
			glfwSetInputMode(Mouse::_window->get(), GLFW_RAW_MOUSE_MOTION, value ? GLFW_TRUE : GLFW_FALSE);
			Mouse::rawInput._enabled = value;
		}
	}
}

bool Mouse::RawInput::supported() noexcept {
	return glfwRawMouseMotionSupported();
}

} // namespace arch::input

