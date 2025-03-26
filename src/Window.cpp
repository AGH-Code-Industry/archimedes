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
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {

	});
}

void Window::resize(int width, int height) const {
	glfwSetWindowSize(_window, width, height);
}

void Window::setTitle(const std::string& title) const {
	glfwSetWindowTitle(_window, title.c_str());
}

int2 Window::getSize() const {
	int2 result;
	glfwGetWindowSize(_window, &result.x, &result.y);
	return result;
}

GLFWwindow* Window::get() const {
	return _window;
}

void Window::swapBuffers() const {
	glfwSwapBuffers(_window);
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(_window);
}

} // namespace arch
