#include <ranges>

#include <input/keyboard/State.h>
#include <input/keyboard/System.h>

namespace arch::input::keyboard::_details {

void System::_init(GLFWwindow* window) noexcept {
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetKeyCallback(window, _callback);
}

void System::_frameBegin() noexcept {
	const auto now = _details::State::Clock::now();
	const auto diff = now - state._lastUpdate;
	state._lastUpdate = now;

	for (auto&& [state, time] : std::views::zip(state._state, state._time)) {
		if (state & KeyState::pressed + KeyState::released) {
			time = {};
		} else {
			time += diff;
		}
	}
}

void System::_frameEnd() noexcept {
	for (auto&& state : state._state) {
		state &= ~(KeyState::pressed + KeyState::released + KeyState::repeat);
	}
}

void System::_callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept {
	auto&& state = _details::state._state[key];
	if (action == GLFW_RELEASE) {
		state = KeyState::released + KeyState::up;
	} else if (action == GLFW_PRESS) {
		state = KeyState::pressed + KeyState::down;
	} else if (action == GLFW_REPEAT) {
		state = KeyState::down + KeyState::repeat;
	}

	state |= mods << KeyState::shift;
}

} // namespace arch::input::keyboard::_details
