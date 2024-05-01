#include "Engine.h"

#include "Exception.h"
#include "InputHandler.h"
#include "Logger.h"
#include "gfx.h"
#include "resource/ModelLoader.h"
#include "resource/TextureLoader.h"

namespace arch {

Engine::Engine(const EngineConfig& config): _engineConfig(config) {}

Engine::~Engine() {
	_shutdown();
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

	// 2D square
	struct Vertex {
		float3 position;
		float3 color;
		float2 tex_coords;
	};

	std::vector<Vertex> vertices{
		{  float3(0.5f,  0.5f, 0.0f), {}, float2(1.0f, 1.0f) },
		{  float3(0.5f, -0.5f, 0.0f), {}, float2(1.0f, 0.0f) },
		{ float3(-0.5f, -0.5f, 0.0f), {}, float2(0.0f, 0.0f) },
		{ float3(-0.5f,	0.5f, 0.0f), {}, float2(0.0f, 1.0f) }
	};
	std::vector<u32> indices{ 0, 1, 3, 1, 2, 3 };

	// Ref<Shader> vShader = Shader::load("shaders/vertex_shader.sprv");
	// Ref<Shader> fShader = Shader::load("shaders/fragment_shader.sprv");
	// Ref<Material> material = Material::create(vShader, fShader);
	// material->setTexture("_mainTxt", TextureLoader::read_file("textures/.jpg"));
	// material->SetFloat("_mixValue", 0.2f);
	// material->SetFloat3("_pos", glm::vec3(0.5f, 0.5f, 0.5f));
	// material->SetColor("_color", glm::vec3(1.0f, 0.0f, 0.0f));

	Ref<Mesh> mesh = Mesh::create(std::span(vertices), indices);

	InputHandler::get().initialize(_mainWindow->get());

	while (!_mainWindow->shouldClose()) {
		_mainWindow->clear(_engineConfig.backgroundColor);

		_renderer->render(mesh, /* material,*/ Mat4x4(1.0f));

		_mainWindow->swapBuffers();
		glfwPollEvents();
	}
}

void Engine::_initialize() {
	_mainWindow = createRef<Window>(_engineConfig.windowWidth, _engineConfig.windowHeight, _engineConfig.windowTitle);

	_renderer = Renderer::create(_engineConfig.renderingApi);
	_renderer->init(_mainWindow);
	_renderer->makeCurrent();

	Logger::info("Engine initialization successful");
}

void Engine::_shutdown() {
	Logger::info("Engine shutdown");
	glfwTerminate();

	_renderer->shutdown();
	_renderer = nullptr;
}

} // namespace arch
