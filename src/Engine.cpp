#include "Engine.h"

#include "Exception.h"
#include "Gfx.h"
#include "Logger.h"
#include "font/FontDB.h"
#include "resource/ModelLoader.h"
#include "resource/TextureLoader.h"
#include "scene/SceneManager.h"
#include <input/keyboard/System.h>
#include <input/mouse/State.h>
#include <input/mouse/System.h>

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

	while (!_mainWindow->shouldClose()) {
		glfwPollEvents();
		input::keyboard::_details::System::_frameBegin();
		input::mouse::_details::System::_frameBegin();

		// Update the application
		_application->update();

		_sceneManager->update();

		// Render the scene
		if (_renderer->beginFrame()) {
			_sceneManager->renderScene(_renderer);

			_renderer->present();
		}

		input::keyboard::_details::System::_frameEnd();
		input::mouse::_details::System::_frameEnd();
	}
}

void Engine::_initialize() {
	_mainWindow = createRef<Window>(_engineConfig.windowWidth, _engineConfig.windowHeight, _engineConfig.windowTitle);

	input::keyboard::_details::System::_init(_mainWindow->get());
	input::mouse::_details::System::_init(_mainWindow);

	_renderer = gfx::Renderer::create(_engineConfig.renderingApi);
	_renderer->init(_mainWindow);
	_renderer->makeCurrent();

	_renderer->setClearColor(_engineConfig.backgroundColor);

	_sceneManager = scene::SceneManager::get();

	_application->init();

	Logger::info("Engine initialization successful");
}

void Engine::_shutdown() {
	_application.reset();
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
