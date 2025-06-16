#include <print>
#include <ranges>

#include <input/keyboard/State.h>
#include <input/keyboard/System.h>

namespace arch::input::keyboard::_details {

void System::_init(GLFWwindow* window) noexcept {
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);
	glfwSetKeyCallback(window, _update);
}

void System::_frameBegin() noexcept {
	const auto now = _details::StateTime::Clock::now();
	const auto diff = now - stateTime._lastUpdate;
	stateTime._lastUpdate = now;

	auto start = _details::StateTime::Clock::now();
	for (auto&& [state, time] : std::views::zip(state._state, stateTime._time)) {
		if (state & *KeyState::changed) {
			if (state & *KeyState::repeat) {
				time += diff;
			} else {
				time = {};
			}
			state &= ~*KeyState::changed;
		} else {
			time += diff;
			state &= ~(KeyState::pressed + KeyState::released + KeyState::repeat);
		}
	}
	auto end = _details::StateTime::Clock::now();
	std::println("{}", std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start));
}

void System::_update(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept {
	auto&& state = keyboard::state._state[key];
	if (action == GLFW_RELEASE) {
		if (state & *KeyState::down) {
			state |= *KeyState::changed;
		}
		state &= *KeyState::changed;
		state |= KeyState::released + KeyState::up;
	} else if (action == GLFW_PRESS) {
		if (state & *KeyState::up) {
			state |= *KeyState::changed;
		}
		state &= *KeyState::changed;
		state |= KeyState::pressed + KeyState::down;
	} else if (action == GLFW_REPEAT) {
		state = KeyState::down + KeyState::repeat + KeyState::changed;
	}

	state |= mods << KeyState::shift;
}

} // namespace arch::input::keyboard::_details
