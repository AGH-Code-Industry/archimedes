#include <ranges>

#include <input/mouse/State.h>
#include <input/mouse/System.h>

namespace arch::input::mouse::_details {

void System::_init(const Ref<Window>& window) noexcept {
	input::mouse::_details::state._init(window);

	glfwSetMouseButtonCallback(window->get(), _buttonCallback);
	glfwSetCursorPosCallback(window->get(), _cursorCallback);
	glfwSetScrollCallback(window->get(), _scrollCallback);
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
		state &= ~(KeyState::pressed + KeyState::released);
	}

	state._dx = state._dy = 0;
	state._xscroll = state._yscroll = 0;
}

void System::_buttonCallback(GLFWwindow* window, int key, int action, int mods) noexcept {
	auto&& state = _details::state._state[key];
	if (action == GLFW_RELEASE) {
		state = KeyState::released + KeyState::up;
	} else if (action == GLFW_PRESS) {
		state = KeyState::pressed + KeyState::down;
	}

	state |= mods << KeyState::shift;
}

void System::_cursorCallback(GLFWwindow* window, double x, double y) noexcept {
	const float actualY = (float)state._window->getSize().y - (float)y;

	state._dx = state._delta * ((float)x - std::exchange(state._x, (float)x));
	state._dy = state._delta * (actualY - std::exchange(state._y, actualY));
	state._delta = true;
}

void System::_scrollCallback(GLFWwindow* window, double x, double y) noexcept {
	state._xscroll = x;
	state._yscroll = y;
}

} // namespace arch::input::mouse::_details
