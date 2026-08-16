#include <GLFW/glfw3.h>
#include <archimedes/exceptions/GLFWException.h>

namespace arch {

GLFWException::GLFWException(const utils::SimpleSourceLocation& loc): Exception("GLFW", "", loc) {
	const char* description;
	int code = glfwGetError(&description);
	_message = "(Code: " + std::to_string(code) + "): " + description;
}

} // namespace arch
