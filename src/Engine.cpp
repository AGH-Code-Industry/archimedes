#include "Engine.h"

#include "ArchMath.h"
#include "InputHandler.h"
#include "Logger.h"
#include "exceptions/GLFWException.h"
#include "resource/ModelLoader.h"
#include "resource/TextureLoader.h"

namespace arch {

Engine::Engine(const EngineConfig& config): _window(1, 1, {}), _engineConfig(config) {}

Engine::~Engine() {
	_terminate();
}

void Engine::start() {
	try {
		_initialize();

		_mainLoop();
	} catch (Exception& e) {
		e.print();
	} catch (std::exception& e) {
		Logger::error("Crashed with exception: {}", e.what());
	} catch (...) {
		Logger::error("Unhandled exception occurred");
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
		float3 position;
		float3 color;
		float2 tex_coords;
	};

	std::vector<Vertex> vertices{
		{ float3(0.5f,  0.5f, 0.0f), {}, float2(1.0f, 1.0f) },
		{ float3(0.5f, -0.5f, 0.0f), {}, float2(1.0f, 0.0f) },
		{ float3(-0.5f, -0.5f, 0.0f), {}, float2(0.0f, 0.0f) },
		{ float3(-0.5f,	0.5f, 0.0f), {}, float2(0.0f, 1.0f) }
	};
	std::vector<u32> indices{ 0, 1, 3, 1, 2, 3 };
	// Model model { { { vertices, indices } } };
	// TextureLoader texture_loader;
	// Renderer3D renderer {};
	// renderer.set_texture(texture_loader.read_file("pawelskrzynski.jpg"));
	// renderer.submit(model);

	InputHandler::get().initialize(_window.get());

	while (!_window.shouldClose()) {
		_window.clear(_engineConfig.background_color);

		// renderer.render();

		_window.swapBuffers();
		glfwPollEvents();
	}
}

void Engine::_initialize() {
	if (!glfwInit()) {
		throw GLFWException();
	}

	_window.resize(_engineConfig.window_width, _engineConfig.window_height);
	_window.setTitle(_engineConfig.window_title);

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
