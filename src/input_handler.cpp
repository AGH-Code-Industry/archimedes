#include "input_handler.h"

namespace arch {
    InputHandler *InputHandler::instance = nullptr;
    std::unordered_map<int, std::function<void()>> InputHandler::key_to_function_table;
    
    InputHandler::InputHandler() {}

    InputHandler& InputHandler::get() {
        if (instance == nullptr)
            instance = new InputHandler();
        return *instance;
    }

    void InputHandler::initialize(GLFWwindow* window) {
        _window = window;
        glfwSetKeyCallback(_window, handle_key_event);
    }

    void InputHandler::bind_key(int keyCode, std::function<void()> func) {
        key_to_function_table[keyCode] = func;
    }

    void InputHandler::handle_key_event(GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto it = key_to_function_table.find(key);
        if (it != key_to_function_table.end()) {
            it->second();
        }
    }
}