#include "input_handler.h"

namespace arch {
    std::unordered_map<int, std::function<void()>> InputHandler::bindedTable;
    GLFWwindow *InputHandler::_window {};
    InputHandler::InputHandler(GLFWwindow *_window) {
        this->_window = _window;
        glfwSetKeyCallback(_window, handleKeyEvent);
        bindKey(GLFW_KEY_ESCAPE, &closeWindow);
    }

    void InputHandler::bindKey(int keyCode, std::function<void()> func) {
        bindedTable[keyCode] = func;
    }

    void InputHandler::closeWindow() {
        glfwSetWindowShouldClose(_window, true);
    }

    void InputHandler::handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (bindedTable.find(key) != bindedTable.end()) {
            bindedTable[key]();
        }
    }
}