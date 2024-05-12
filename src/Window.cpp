#include "Window.h"

#include "exceptions/GLFWException.h"
#include <GLFW/glfw3.h>

namespace arch {

Window::Window(int width, int height, const std::string& name, GLFWmonitor* monitor, const Window& share) {
	_title = name;
	_initialize(width, height, _title.c_str(), monitor, share._window);
}

Window::Window(int width, int height, const std::string& name, GLFWmonitor* monitor) {
	_title = name;
	_initialize(width, height, _title.c_str(), monitor, nullptr);
}

void Window::_initialize(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow* window) {
	if (!glfwInit()) {
		throw GLFWException();
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	_window = glfwCreateWindow(width, height, name, monitor, window);

	if (!_window) {
		glfwTerminate();
		throw GLFWException();
	}

	glfwMakeContextCurrent(_window);

	glfwSetFramebufferSizeCallback(_window, _framebufferSizeCallback);
}

void Window::resize(int width, int height) const {
	glfwSetWindowSize(_window, width, height);
}

void Window::setTitle(const std::string& title) const {
	glfwSetWindowTitle(_window, title.c_str());
}

GLFWwindow* Window::get() const {
	return _window;
}

void Window::_framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void Window::swapBuffers() const {
	glfwSwapBuffers(_window);
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(_window);
}

void Window::clear(Color color) const {
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::clear(float r, float g, float b, float a) const {
	Color color(r, g, b, a);
	clear(color);
}

} // namespace arch
