#pragma once

#include <string>

#include "Application.h"
#include "Ref.h"
#include "Window.h"
#include "gfx/Renderer.h"
#include <gtest/gtest_prod.h>

namespace arch {
namespace scene {
class SceneManager;
}

struct EngineConfig {
	int windowWidth;
	int windowHeight;
	std::string windowTitle;
	Color backgroundColor;
	gfx::RenderingAPI renderingApi;
};

/**
 * Main class of Archimedes project.
 */
class Engine {
public:
	Engine(const EngineConfig& config, Unique<Application> application);
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
