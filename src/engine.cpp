#include "engine.h"

#include <Logger.h>
#include <exceptions/GLFWException.h>

#include <glm/glm.hpp>

#include "input_handler.h"
#include "resource/model_loader.h"
#include "resource/texture_loader.h"

using namespace arch;

Engine::Engine(EngineConfig config):
	_window(1, 1, {}),
	_engine_config(config),
	_renderer() {}

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
    Logger::info("Starting engine main loop");
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
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 tex_coords;
	};
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
    // Model model { { { vertices, indices } } };
    // TextureLoader texture_loader;
    // Renderer3D renderer {};
    //renderer.set_texture(texture_loader.read_file("pawelskrzynski.jpg"));
    // renderer.submit(model);

    InputHandler::get().initialize(_window.get());

    while (!_window.should_close()) {
        _window.clear(_engine_config.background_color);

        // renderer.render();
        
        _window.swap_buffers();
        glfwPollEvents();
    }
}

void Engine::initialize() {
    if (!glfwInit())
        throw GLFWException();

    _window.resize(_engine_config.window_width, _engine_config.window_height);
    _window.set_title(_engine_config.window_title);

	_renderer.init();

    // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    //     throw GladException();
    // }
    //
    // AssimpInitializer::init();

	Logger::info("Engine initialization successful");
}

void Engine::terminate() {
    glfwTerminate();
}