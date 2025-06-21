#pragma once

#include <Engine.h>
#include <GLFW/glfw3.h>

namespace arch::input::mouse::_details {

class System {
	friend class ::arch::Engine;

	static void _init(const Ref<Window>& window) noexcept;
	static void _buttonCallback(GLFWwindow* window, int key, int action, int mods) noexcept;
	static void _cursorCallback(GLFWwindow* window, double x, double y) noexcept;
	static void _scrollCallback(GLFWwindow* window, double x, double y) noexcept;
	static void _frameBegin() noexcept;
	static void _frameEnd() noexcept;
};

} // namespace arch::input::mouse::_details
