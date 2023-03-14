//
// Created by tosiek on 22.02.23.
//

#include "engine.h"

using namespace arch;
using json = nlohmann::json;

Engine::~Engine() {
    terminate();
}


void Engine::start() {
    initialize();

    main_loop();

    terminate();
}


void Engine::main_loop() {
    while(!_window.should_close())
    {
        process_input();

        _window.clear(_engineConfig.backgroundColor);
        _window.swap_buffers();
        glfwPollEvents();
    }
}

void Engine::initialize() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    load_configuration();
    _window = Window(_engineConfig.width, _engineConfig.height, _engineConfig.windowTitle, nullptr);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw InitException();
    }
}

void Engine::terminate() {
    glfwTerminate();
}

void Engine::process_input() {
    if(glfwGetKey(_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(_window.get(), true);
    }
}

void Engine::load_configuration(){
    try {
        std::ifstream windowConfigFile("config/engine-config/window.json");
        json windowJsonData = json::parse(windowConfigFile);

        _engineConfig.width = windowJsonData["width"];
        _engineConfig.height = windowJsonData["height"];
        _engineConfig.windowTitle = windowJsonData["window_title"];

        auto background_color_list = windowJsonData["background_color"];

        float r = background_color_list.at(0);
        float g = background_color_list.at(1);
        float b = background_color_list.at(2);
        float a = background_color_list.at(3);

        _engineConfig.backgroundColor = glm::vec4(r, g, b, a);
    } catch(json::exception &e) {
        throw ConfigException("wrong JSON file for engine config (" + std::string(e.what()) + ")");
    } catch(std::ifstream::failure &e) {
        throw ConfigException("cannot open engine config file (" + std::string(e.what()) + ")");
    }
}
