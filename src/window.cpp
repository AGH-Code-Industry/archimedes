#include "window.h"
#include "graphics/glfw_exception.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace arch {

Window::Window(int width, int height, const std::string& name, GLFWmonitor *monitor, Window &share) {
    _title = name;
    initialize(width, height, _title.c_str(), monitor, share._window);
}

Window::Window(int width, int height, const std::string& name, GLFWmonitor *monitor) {
    _title = name;
    initialize(width, height, _title.c_str(), monitor, nullptr);
}

void Window::initialize(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow *window) {
    if (!glfwInit())
        throw GLFWException();
        
    _window = glfwCreateWindow(width, height, name, monitor, window);

    if (!_window) {
        glfwTerminate();
        throw GLFWException();
    }

    glfwMakeContextCurrent(_window);
	gladLoadGL();
    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
}

void Window::resize(int width, int height) {
    glfwSetWindowSize(_window, width, height);
}

void Window::set_title(const std::string &title) {
    glfwSetWindowTitle(_window, title.c_str());
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
}

void Window::clear(float r, float g, float b, float a) {
    glm::vec4 color(r,g,b,a);
    clear(color);
}

}
