#include <archimedes/Engine.h>
#include <archimedes/Exception.h>
#include <archimedes/Gfx.h>
#include <archimedes/Logger.h>
#include <archimedes/font/FontDB.h>
#include <archimedes/input/System.h>
#include <archimedes/resource/ModelLoader.h>
#include <archimedes/resource/TextureLoader.h>
#include <archimedes/scene/SceneManager.h>

namespace arch {

Engine::Engine(const EngineConfig& config, Unique<Application>& application):
	_engineConfig{ config },
	_application{ std::move(application) } {}

Engine::~Engine() {
	_shutdown();
}

void Engine::start() {
	auto run = [&] {
		_initialize();
		_mainLoop();
	};

	if (_engineConfig.noCatch) {
		run();
	} else {
		try {
			run();
		} catch (Exception& e) {
			log::error("{:0}", e); // print full stacktrace
		} catch (std::exception& e) {
			log::error("Crashed with exception: {}", e.what());
		} catch (...) {
			log::error("Unhandled exception occurred");
		}
	}
}

void Engine::_mainLoop() {
	log::info("Starting engine main loop");

	while (!_mainWindow->shouldClose()) {
		glfwPollEvents();
		input::System::_frameBegin();

		// Update the application
		_application->update();

		_sceneManager->update();

		// Render the scene
		if (_renderer->beginFrame()) {
			_sceneManager->renderScene(_renderer);

			_renderer->present();
		}

		input::System::_frameEnd();
	}
}

void Engine::_initialize() {
	log::_details::LoggerSingleton::init(_engineConfig.loggerConfig);

	_mainWindow = createRef<Window>(_engineConfig.windowWidth, _engineConfig.windowHeight, _engineConfig.windowTitle);

	input::System::_init(_mainWindow);

	_renderer = gfx::Renderer::create(_engineConfig.renderingApi);
	_renderer->init(_mainWindow);
	_renderer->makeCurrent();

	_renderer->setClearColor(_engineConfig.backgroundColor);

	_sceneManager = scene::SceneManager::get();

	_application->init();

	log::info("Engine initialization successful");
}

void Engine::_shutdown() {
	_application.reset();
	font::FontDB::_singleton.reset();
	scene::SceneManager::get()->shutdown();

	log::info("Engine shutingdown");
	glfwTerminate();

	if (_renderer) {
		log::info("Shutingdown renderer");
		_renderer->shutdown();
		_renderer = nullptr;
	} else {
		log::info("Renderer is already shutdown");
	}

	log::info("Engine shutdown");
}

} // namespace arch
