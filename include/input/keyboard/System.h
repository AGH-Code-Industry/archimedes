#pragma once

#include <Engine.h>
#include <GLFW/glfw3.h>

namespace arch::input::keyboard::_details {

class System {
	friend class ::arch::Engine;

	static void _init(GLFWwindow* window) noexcept;
	static void _update(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
	static void _frameBegin() noexcept;
};

} // namespace arch::input::keyboard::_details
