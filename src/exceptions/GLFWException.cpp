#include <GLFW/glfw3.h>
#include <archimedes/exceptions/GLFWException.h>

namespace arch {

GLFWException::GLFWException(const std::source_location& location): Exception("GLFW", location) {
	const char* description;
	int code = glfwGetError(&description);
	_message = "(Code: " + std::to_string(code) + "): " + description;
}

} // namespace arch
