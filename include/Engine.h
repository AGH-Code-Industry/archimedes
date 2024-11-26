#pragma once

#include <string>

#include "Application.h"
#include "Ref.h"
#include "Window.h"
#include "gfx/Renderer.h"
#include <gtest/gtest_prod.h>

namespace arch {

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
	Engine(const EngineConfig& config, const Ref<Application>& application);
	~Engine();

	/**
	 * Only function visible for user.
	 */
	void start();

private:
	Ref<Window> _mainWindow;
	EngineConfig _engineConfig;
	Ref<gfx::Renderer> _renderer;
	Ref<Application> _application;

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
