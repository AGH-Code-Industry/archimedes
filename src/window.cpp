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

void Window::swap_buffers(){
    glfwSwapBuffers(_window);
}

bool Window::should_close() {
    return glfwWindowShouldClose(_window);
}

void Window::clear(glm::vec4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Window::clear(float r, float g, float b, float a) {
    glm::vec4 color(r,g,b,a);
    clear(color);
}
