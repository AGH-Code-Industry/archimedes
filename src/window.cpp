//
// Created by tosiek on 22.02.23.
//

#include "window.h"

using namespace arch;

Window::Window(int width, int height, const std::string& name, GLFWmonitor *monitor, Window &share) {
    _window = glfwCreateWindow(width, height, name.c_str(), monitor, share._window);
}

Window::Window(int width, int height, const std::string& name, GLFWmonitor *monitor) {
    _window = glfwCreateWindow(width, height, name.c_str(), monitor, nullptr);
}

GLFWwindow *Window::get() {
    return _window;
}
