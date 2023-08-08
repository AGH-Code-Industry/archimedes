#include "input_handler.h"

namespace arch {
    InputHandler InputHandler::inputHandlerInstance;
    std::unordered_map<int, std::function<void()>> InputHandler::bindedTable;
    GLFWwindow *InputHandler::_window {};
    
    InputHandler::InputHandler() {}

    InputHandler& InputHandler::get() {
        return inputHandlerInstance;
    }

    void InputHandler::setCurrentWindow(GLFWwindow* _window) {
        this->_window = _window;
        glfwSetKeyCallback(_window, handleKeyEvent);
        bindAll();
    }

    void InputHandler::bindAll() {
        bindKey(GLFW_KEY_ESCAPE, &closeWindow);
        bindKey(GLFW_KEY_F, &maximizeWindow);
        bindKey(GLFW_KEY_M, &restoreWindow);
    }

    void InputHandler::bindKey(int keyCode, std::function<void()> func) {
        bindedTable[keyCode] = func;
    }

    void InputHandler::closeWindow() {
        glfwSetWindowShouldClose(_window, true);
    }

    void InputHandler::maximizeWindow() {
        glfwMaximizeWindow(_window);
    }

    void InputHandler::iconifyWindow() {
        glfwIconifyWindow(_window);
    }

    void InputHandler::restoreWindow() {
        glfwRestoreWindow(_window);
    }

    void InputHandler::handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (bindedTable.find(key) != bindedTable.end()) {
            bindedTable[key]();
        }
    }
}