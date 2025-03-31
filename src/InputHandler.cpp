#include "InputHandler.h"

namespace arch {

InputHandler* InputHandler::instance = nullptr;
std::unordered_map<int, std::function<void(int)>> InputHandler::keyToFunctionTable;

InputHandler::InputHandler() {}

InputHandler& InputHandler::get() {
	if (instance == nullptr) {
		instance = new InputHandler();
	}
	return *instance;
}

void InputHandler::initialize(GLFWwindow* window) {
	_window = window;
	glfwSetKeyCallback(_window, _handleKeyEvent);
}

void InputHandler::bindKey(int keyCode, const std::function<void(int)>& func) const {
	keyToFunctionTable[keyCode] = func;
}

void InputHandler::_handleKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto it = keyToFunctionTable.find(key);
	if (it != keyToFunctionTable.end()) {
		it->second(action);
	}
}

} // namespace arch
