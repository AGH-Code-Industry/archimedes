//
// Created by tosiek on 22.02.23.
//

#include "engine.h"

using namespace arch;

void Engine::start() {
    initialize();

    main_loop();

    terminate();
}


void Engine::main_loop() {
    while(!_window.should_close())
    {
        process_input();

        _window.clear(_background_color);
        _window.swap_buffers();
        glfwPollEvents();
    }
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

void Engine::terminate() {
    glfwTerminate();
}

void Engine::process_input() {
    if(glfwGetKey(_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(_window.get(), true);
    }
}
