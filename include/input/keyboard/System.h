#pragma once

#include <Engine.h>
#include <GLFW/glfw3.h>

namespace arch::input::keyboard::_details {

/// @brief System for keyboard state
class System {
	friend class ::arch::Engine;

	// inits system (callbacks, etc.)
	static void _init(GLFWwindow* window) noexcept;
	// GLFW callback
	static void _callback(GLFWwindow* window, int key, int scancode, int action, int mods) noexcept;
	static void _frameBegin() noexcept;
	static void _frameEnd() noexcept;
};

} // namespace arch::input::keyboard::_details
