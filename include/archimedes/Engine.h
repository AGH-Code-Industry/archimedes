#pragma once

#include <string>

#include "Application.h"
#include "Ref.h"
#include "Window.h"
#include "gfx/Renderer.h"
#include <archimedes/Logger.h>
#include <gtest/gtest_prod.h>

namespace arch {
namespace scene {
class SceneManager;
}

/// @brief Logger config
struct LoggerConfig {
	/// @brief Logger name
	std::string name;
	/// @brief Logger level
	log::Level level;
	/// @brief Whether to log to a file
	bool file;
};

/// @brief Engine configuration
struct EngineConfig {
	/// @brief Initial window width
	int windowWidth;
	/// @brief Initial window height
	int windowHeight;
	/// @brief Initial window title
	std::string windowTitle;
	/// @brief Background color
	Color backgroundColor;
	/// @brief Rendering API to use (currently only Nvrhi_VK is supported)
	gfx::RenderingAPI renderingApi;
	/// @brief Logger config
	LoggerConfig loggerConfig = { .name = "Archimedes", .level = log::info, .file = true };
	/// @brief Whether engine passes or captures exceptions
	bool noCatch = false;
};

/**
 * Main class of Archimedes project.
 */
class Engine {
public:
	Engine(const EngineConfig& config, Unique<Application>& application);
	~Engine();

	/**
	 * Only function visible for user.
	 */
	void start();

private:

	EngineConfig _engineConfig;
	Ref<Window> _mainWindow;
	Ref<gfx::Renderer> _renderer;
	Ref<scene::SceneManager> _sceneManager;
	Unique<Application> _application;

private:
	/**
	 * Responsible for drawing game on the screen.
	 */
	void _mainLoop();

	/**
	 * Initialize glfw in engine and creates window.
	 */
	void _initialize();

	/**
	 * Clearing all previously allocated GLFW and Engine resources.
	 */
	void _shutdown();

private:
	FRIEND_TEST(EngineTest, ConfigWindowTest);
};

} // namespace arch
