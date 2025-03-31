#pragma once

#include <functional>

#include <GLFW/glfw3.h>

namespace arch {
class InputHandler {
public:
	static InputHandler& get();

	/**
	 * Sets InputHandler to specific window and initializes key-function binds.
	 */
	void initialize(GLFWwindow* window);
	/**
	 * Binds specific key code to a function.
	 */
	void bindKey(int keyCode, const std::function<void()>& func) const;

private:
	InputHandler();
	static InputHandler* instance;
	GLFWwindow* _window{};
	static std::unordered_map<int, std::function<void()>> keyToFunctionTable;

	/**
	 * Callback function that runs when key is pressed or released.
	 */
	static void _handleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
};
} // namespace arch
