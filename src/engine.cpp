#include "engine.h"
#include "graphics/model.h"
#include "graphics/primitives.h"
#include "graphics/renderer.h"
#include "graphics/glfw_exception.h"
#include "graphics/glad_exception.h"

#include <spdlog/spdlog.h>

using namespace arch;
using json = nlohmann::json;

Engine::Engine() : _window(1, 1, {}) {}

Engine::~Engine() {
    terminate();
}


void Engine::start() {
    initialize();

    main_loop();

    terminate();
}


void Engine::main_loop() {
    spdlog::info("Starting engine main loop");
    std::vector<Vertex> vertices {
        { glm::vec3(0.5f, 0.5f, 0.5f), {}, {}},
        { glm::vec3(-0.5f, 0.5f, 0.5f), {}, {}},
        { glm::vec3(-0.5f, -0.5f, 0.5f), {}, {}},
        { glm::vec3(0.5f, -0.5f, 0.5f), {}, {}},
        { glm::vec3(0.5f, 0.5f, -0.5f), {}, {}},
        { glm::vec3(-0.5f, 0.5f, -0.5f), {}, {}},
        { glm::vec3(-0.5f, -0.5f, -0.5f), {}, {}},
        { glm::vec3(0.5f, -0.5f, -0.5f), {}, {}}
    };
    std::vector<Index> indices { 
        0, 1, 2, 0, 3, 2,
        4, 5, 6, 4, 7, 6,
        4, 0, 3, 4, 7, 3,
        5, 1, 2, 5, 6, 2,
        7, 6, 2, 7, 3, 2,
        4, 5, 1, 5, 0, 1
    };
    Model model { { { vertices, indices, {} } } };
    Renderer3D renderer {};
    renderer.submit(model);
    while(!_window.should_close())
    {
        process_input();
        _window.clear(_engine_config.background_color);

        renderer.render();
        
        _window.swap_buffers();
        glfwPollEvents();
    }
}

void Engine::initialize() {
    if (!glfwInit())
        throw GLFWException();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    load_configuration();
    _window.resize(_engine_config.window_width, _engine_config.window_height);
    _window.set_title(_engine_config.window_title);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw GladException();
    }

    spdlog::info("Engine initialization successful");
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

        _engine_config.window_width = windowJsonData["width"];
        _engine_config.window_height = windowJsonData["height"];
        _engine_config.window_title = windowJsonData["window_title"];

        auto background_color_list = windowJsonData["background_color"];

        float r = background_color_list.at(0);
        float g = background_color_list.at(1);
        float b = background_color_list.at(2);
        float a = background_color_list.at(3);

        _engine_config.background_color = glm::vec4(r, g, b, a);
    } catch(json::exception &e) {
        throw ConfigException("wrong JSON file for engine config (" + std::string(e.what()) + ")");
    } catch(std::ifstream::failure &e) {
        throw ConfigException("cannot open engine config file (" + std::string(e.what()) + ")");
    }
}
