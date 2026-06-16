#include <GLFW/glfw3.h>
#include <archimedes/Window.h>
#include <archimedes/exceptions/GLFWException.h>
#include <archimedes/gfx/Renderer.h>

namespace arch {

Window::Window(int width, int height, const std::string& name, const Window& share) {
	_title = name;
	_initialize(width, height, _title.c_str(), share._window);
}

Window::Window(int width, int height, const std::string& name) {
	_title = name;
	_initialize(width, height, _title.c_str(), nullptr);
}

void Window::_initialize(int width, int height, const char* name, GLFWwindow* window) {
	if (!glfwInit()) {
		throw GLFWException();
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	_window = glfwCreateWindow(width, height, name, NULL, window);
	_size = { width, height };
	_sizeWindowed = { width, height };

	if (!_window) {
		glfwTerminate();
		throw GLFWException();
	}

	glfwMakeContextCurrent(_window);
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {});
	glfwSetWindowUserPointer(_window, this);
	glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
		auto win = (Window*)glfwGetWindowUserPointer(window);
		win->_size = { width, height };
		if (win->mode() == windowed) {
			win->_sizeWindowed = { width, height };
		}
	});
}

void Window::resize(int width, int height) {
	glfwSetWindowSize(_window, width, height);
}

void Window::setTitle(const std::string& title) {
	glfwSetWindowTitle(_window, title.c_str());
}

void Window::setFullscreen() {
	setFullscreen(Monitor::get());
}

void Window::setFullscreen(Monitor& monitor) {
	_mode = fullscreen;
	glfwSetWindowMonitor(
		_window,
		monitor._monitor,
		0,
		0,
		monitor.originalSize().x,
		monitor.originalSize().y,
		monitor.originalRefreshRate()
	);
}

void Window::setWindowed() {
	_mode = windowed;
	glfwSetWindowMonitor(_window, nullptr, 100, 100, _sizeWindowed.x, _sizeWindowed.y, 0);
}

void Window::toggleFullscreen() {
	toggleFullscreen(Monitor::get());
}

void Window::toggleFullscreen(Monitor& monitor) {
	if (_mode == windowed) {
		setFullscreen(monitor);
	} else {
		setWindowed();
	}
}

GLFWwindow* Window::get() const {
	return _window;
}

int2 Window::size() const {
	return _size;
}

Window::Mode Window::mode() const {
	return _mode;
}

void Window::swapBuffers() const {
	glfwSwapBuffers(_window);
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(_window);
}

} // namespace arch
