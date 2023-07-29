#include "engine.h"
#include "graphics/model.h"
#include "graphics/primitives.h"
#include "graphics/renderer.h"
#include "graphics/glfw_exception.h"
#include "graphics/glad_exception.h"
#include "resource/texture_loader.h"
#include "resource/model_loader.h"
#include "input_handler.h"

#include <spdlog/spdlog.h>

using namespace arch;

Engine::Engine(EngineConfig config) : 
_window(1, 1, {}),
_engine_config(config) {}

Engine::~Engine() {
    terminate();
}


void Engine::start() {
    initialize();

    try {
        main_loop();
    } catch (std::exception &e) {
        spdlog::error("Crashed with exception: {}", e.what());
    } catch (...) {
        spdlog::error("Dupa");
    }
}


void Engine::main_loop() {
    spdlog::info("Starting engine main loop");
    // 3D cube
    // std::vector<Vertex> vertices {
    //     { glm::vec3(0.5f, 0.5f, 0.5f), {}, {}},
    //     { glm::vec3(-0.5f, 0.5f, 0.5f), {}, {}},
    //     { glm::vec3(-0.5f, -0.5f, 0.5f), {}, {}},
    //     { glm::vec3(0.5f, -0.5f, 0.5f), {}, {}},
    //     { glm::vec3(0.5f, 0.5f, -0.5f), {}, {}},
    //     { glm::vec3(-0.5f, 0.5f, -0.5f), {}, {}},
    //     { glm::vec3(-0.5f, -0.5f, -0.5f), {}, {}},
    //     { glm::vec3(0.5f, -0.5f, -0.5f), {}, {}}
    // };
    // std::vector<uint32_t> indices { 
    //     0, 1, 2, 0, 3, 2,
    //     4, 5, 6, 4, 7, 6,
    //     4, 0, 3, 4, 7, 3,
    //     5, 1, 2, 5, 6, 2,
    //     7, 6, 2, 7, 3, 2,
    //     4, 5, 1, 5, 0, 1
    // };
    // 2D square
    std::vector<Vertex> vertices {
        { glm::vec3(0.5f, 0.5f, 0.0f), {}, glm::vec2(1.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f), {}, glm::vec2(1.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.0f), {}, glm::vec2(0.0f, 0.0f) },
        { glm::vec3(-0.5f, 0.5f, 0.0f), {}, glm::vec2(0.0f, 1.0f) }
    };
    std::vector<uint32_t> indices {
        0, 1, 3,
        1, 2, 3
    };
    Model model { { { vertices, indices } } };
    TextureLoader texture_loader;
    Renderer3D renderer {};
    renderer.set_texture(texture_loader.read_file("pawelskrzynski.jpg"));
    renderer.submit(model);

    InputHandler input_handler(_window.get());
    while (!_window.should_close()) {
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

    _window.resize(_engine_config.window_width, _engine_config.window_height);
    _window.set_title(_engine_config.window_title);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw GladException();
    }

    AssimpInitializer::init();

    spdlog::info("Engine initialization successful");
}

void Engine::terminate() {
    glfwTerminate();
}