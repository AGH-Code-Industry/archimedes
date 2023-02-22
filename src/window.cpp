//
// Created by tosiek on 22.02.23.
//

#include "window.h"

using namespace arch;

Window::Window(int width, int height, const std::string& name, GLFWmonitor *monitor, Window &share) {
    initialize(width, height, name.c_str(), monitor, share._window);
}

Window::Window(int width, int height, const std::string& name, GLFWmonitor *monitor) {
    initialize(width, height, name.c_str(), monitor, nullptr);
}

void Window::initialize(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow *window) {
    _window = glfwCreateWindow(width, height, name, monitor, window);

    if (!_window) {
        glfwTerminate();
        throw InitException("Window cannot be created");
    }

    glfwMakeContextCurrent(_window);

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
}

GLFWwindow *Window::get() {
    return _window;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}