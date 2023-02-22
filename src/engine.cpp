//
// Created by tosiek on 22.02.23.
//

#include "engine.h"

using namespace arch;

void Engine::start() {

}


void Engine::main_loop() {

}

void Engine::initialize() {
    if (!glfwInit())
        throw InitException();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    _window = Window(1920, 1080, "MyGame", nullptr);
}
