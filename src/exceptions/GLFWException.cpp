#include "exceptions/GLFWException.h"

#include <GLFW/glfw3.h>

namespace arch {

GLFWException::GLFWException(): Exception("GLFW") {
	const char* description;
	int code = glfwGetError(&description);
	append_msg("code ");
	append_msg(std::to_string(code));
	append_msg(" description ");
	append_msg(description);
}

} // namespace arch
