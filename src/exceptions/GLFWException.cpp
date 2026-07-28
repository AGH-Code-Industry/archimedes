#include <GLFW/glfw3.h>
#include <archimedes/exceptions/GLFWException.h>

namespace arch {

GLFWException::GLFWException(const std::stacktrace& stacktrace): Exception("GLFW", "", stacktrace) {
	const char* description;
	int code = glfwGetError(&description);
	_message = "(Code: " + std::to_string(code) + "): " + description;
}

} // namespace arch
