#include "Engine.h"

#include "Exception.h"
#include "Gfx.h"
#include "InputHandler.h"
#include "Logger.h"
#include "font/FontDB.h"
#include "resource/ModelLoader.h"
#include "resource/TextureLoader.h"
#include "scene/SceneManager.h"

namespace arch {

Engine::Engine(const EngineConfig& config, Unique<Application>& application):
	_engineConfig{ config },
	_application{ std::move(application) } {}

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

	InputHandler::get().initialize(_mainWindow->get());

	while (!_mainWindow->shouldClose()) {
		// Update the application
		_application->update();

		_sceneManager->update();

		// Render the scene
		if (_renderer->beginFrame()) {
			_sceneManager->renderScene(_renderer);

			_renderer->present();
		}

		glfwPollEvents();
	}
}

void Engine::_initialize() {
	_mainWindow = createRef<Window>(_engineConfig.windowWidth, _engineConfig.windowHeight, _engineConfig.windowTitle);

	_renderer = gfx::Renderer::create(_engineConfig.renderingApi);
	_renderer->init(_mainWindow);
	_renderer->makeCurrent();

	_renderer->setClearColor(_engineConfig.backgroundColor);

	_sceneManager = scene::SceneManager::get();

	_application->init();

	Logger::info("Engine initialization successful");
}

void Engine::_shutdown() {
	font::FontDB::_singleton.reset();
	scene::SceneManager::get()->shutdown();

	Logger::info("Engine shutingdown");
	glfwTerminate();

	if (_renderer) {
		Logger::info("Shutingdown renderer");
		_renderer->shutdown();
		_renderer = nullptr;
	} else {
		Logger::info("Renderer is already shutdown");
	}

	Logger::info("Engine shutdown");
}

} // namespace arch
