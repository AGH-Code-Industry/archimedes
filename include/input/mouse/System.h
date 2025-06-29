#pragma once

#include <Engine.h>
#include <GLFW/glfw3.h>

namespace arch::input::mouse::_details {

/// @brief System for keyboard state
class System {
	friend class ::arch::Engine;

	// inits system (callbacks, etc.)
	static void _init(const Ref<Window>& window) noexcept;
	// GLFW button callback
	static void _buttonCallback(GLFWwindow* window, int key, int action, int mods) noexcept;
	// GLFW mouse pos callback
	static void _cursorCallback(GLFWwindow* window, double x, double y) noexcept;
	// GLFW scroll callback
	static void _scrollCallback(GLFWwindow* window, double x, double y) noexcept;
	static void _frameBegin() noexcept;
	static void _frameEnd() noexcept;
};

} // namespace arch::input::mouse::_details
