#include "exceptions/GLFWException.h"

#include <GLFW/glfw3.h>

namespace arch {

GLFWException::GLFWException(): Exception("GLFW") {
	const char* description;
	int code = glfwGetError(&description);
	_appendMsg("code ");
	_appendMsg(std::to_string(code));
	_appendMsg(" description ");
	_appendMsg(description);
}

} // namespace arch
