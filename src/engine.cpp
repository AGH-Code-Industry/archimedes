#include "engine.h"

#include <glm/glm.hpp>

#include "InputHandler.h"
#include "resource/ModelLoader.h"
#include "resource/TextureLoader.h"
#include <Logger.h>
#include <exceptions/GLFWException.h>

namespace arch {

Engine::Engine(const EngineConfig& config): _window(1, 1, {}), _engine_config(config), _renderer() {}

Engine::~Engine() {
	_terminate();
}

void Engine::start() {
	_initialize();

	try {
		_mainLoop();
	} catch (std::exception& e) {
		Logger::error("Crashed with exception: {}", e.what());
	} catch (...) {
		Logger::error("Dupa");
	}
}

void Engine::_mainLoop() {
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

	std::vector<Vertex> vertices{
		{ glm::vec3(0.5f,	 0.5f, 0.0f), {}, glm::vec2(1.0f, 1.0f)},
		{ glm::vec3(0.5f, -0.5f, 0.0f), {}, glm::vec2(1.0f, 0.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f), {}, glm::vec2(0.0f, 0.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f), {}, glm::vec2(0.0f, 1.0f)}
	};
	std::vector<uint32_t> indices{0, 1, 3, 1, 2, 3};
	// Model model { { { vertices, indices } } };
	// TextureLoader texture_loader;
	// Renderer3D renderer {};
	// renderer.set_texture(texture_loader.read_file("pawelskrzynski.jpg"));
	// renderer.submit(model);

	InputHandler::get().initialize(_window.get());

	while (!_window.shouldClose()) {
		_window.clear(_engine_config.background_color);

		// renderer.render();

		_window.swapBuffers();
		glfwPollEvents();
	}
}

void Engine::_initialize() {
	if (!glfwInit()) {
		throw GLFWException();
	}

	_window.resize(_engine_config.window_width, _engine_config.window_height);
	_window.setTitle(_engine_config.window_title);

	_renderer.init();

	// if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	//     throw GladException();
	// }
	//
	// AssimpInitializer::init();

	Logger::info("Engine initialization successful");
}

void Engine::_terminate() {
	glfwTerminate();
}

} // namespace arch
