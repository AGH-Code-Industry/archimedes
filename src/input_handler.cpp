#include "input_handler.h"
#include <GLFW/glfw3.h>

namespace arch {
    InputHandler::InputHandler(GLFWwindow *_window) {
        this->_window = _window;
        glfwSetKeyCallback(_window, keyCallback);
    }

    void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}