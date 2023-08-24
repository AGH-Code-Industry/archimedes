#include "graphics/glfw_exception.h"

#include <GLFW/glfw3.h>

#include <sstream>

namespace arch {

GLFWException::GLFWException() : Exception("GLFW") {
    const char *description;
    int code = glfwGetError(&description);
    std::stringstream ss;
    ss << "code " << code << " description " << description;
    set_description(ss.str());
}

}
