#pragma once

#include <Engine.h>
#include <GLFW/glfw3.h>
#include <Ref.h>

namespace arch::input {

/// @brief Input system class
class System {
	friend class ::arch::Engine;

	// initializes input state of window
	static void _init(const Ref<Window>& window) noexcept;

	// called before each frame
	static void _frameBegin() noexcept;
	// called after each frame
	static void _frameEnd() noexcept;

	// GLFW callbacks
	static void _keyboardKeyCallback(GLFWwindow* window, int keyCode, int scancode, int action, int mods) noexcept;
	static void _mouseButtonCallback(GLFWwindow* window, int buttonCode, int action, int mods) noexcept;
	static void _mouseCursorCallback(GLFWwindow* window, double x, double y) noexcept;
	static void _mouseScrollCallback(GLFWwindow* window, double x, double y) noexcept;
};

} // namespace arch::input
