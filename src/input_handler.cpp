#include "input_handler.h"

namespace arch {
    InputHandler InputHandler::input_handler_instance;
    std::unordered_map<int, std::function<void()>> InputHandler::binded_table;
    GLFWwindow *InputHandler::_window {};
    
    InputHandler::InputHandler() {}

    InputHandler& InputHandler::get() {
        return input_handler_instance;
    }

    void InputHandler::set_current_window(GLFWwindow* window) {
        _window = window;
        glfwSetKeyCallback(_window, handle_key_event);
        bind_all();
    }

    void InputHandler::bind_all() {
        bind_key(GLFW_KEY_ESCAPE, &close_window);
        bind_key(GLFW_KEY_F, &maximize_window);
        bind_key(GLFW_KEY_M, &restore_window);
    }

    void InputHandler::bind_key(int keyCode, std::function<void()> func) {
        binded_table[keyCode] = func;
    }

    void InputHandler::close_window() {
        glfwSetWindowShouldClose(_window, true);
    }

    void InputHandler::maximize_window() {
        glfwMaximizeWindow(_window);
    }

    void InputHandler::iconify_window() {
        glfwIconifyWindow(_window);
    }

    void InputHandler::restore_window() {
        glfwRestoreWindow(_window);
    }

    void InputHandler::handle_key_event(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto it = binded_table.find(key);
        if (it != binded_table.end()) {
            it->second();
        }
    }
}