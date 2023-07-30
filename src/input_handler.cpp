#include "input_handler.h"

#include <GLFW/glfw3.h>
#include <window.h>
#include <functional>

namespace arch {
    InputHandler::bindedPair InputHandler::bindedPairArray[BINDED_PAIR_ARRAY_SIZE] {};
    InputHandler::InputHandler(GLFWwindow *_window) {
        this->_window = _window;
        initializeBindedPairArray();
        glfwSetKeyCallback(_window, handleKeyEvent);
    }

    void InputHandler::bindKey(int keyCode, void (*func)()) {
        for (int i=0; i<BINDED_PAIR_ARRAY_SIZE; i++) {
            if (bindedPairArray[i].keyCode == GLFW_KEY_UNKNOWN) {
                bindedPairArray[i].keyCode = keyCode;
                bindedPairArray[i].func_pointer = func;
                break;
            }
        }
    }

    void InputHandler::handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {
        for (int i=0; i<BINDED_PAIR_ARRAY_SIZE; i++) {
            if (bindedPairArray[i].keyCode == key) {
                bindedPairArray[i].func_pointer();
                break;
            }
        }
    }

    void InputHandler::initializeBindedPairArray() {
        for (int i=0; i<BINDED_PAIR_ARRAY_SIZE; i++) {
            bindedPairArray[i].keyCode = GLFW_KEY_UNKNOWN;
            bindedPairArray[i].func_pointer = nullptr;
        }
    }
}