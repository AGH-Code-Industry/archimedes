#pragma once

#include <memory>
#include <string>

#include "Application.h"
#include "Ref.h"
#include "Window.h"
#include "gfx/Renderer.h"
#include <gtest/gtest_prod.h>

namespace arch {

struct EngineConfig {
	int window_width;
	int window_height;
	std::string window_title;
	glm::vec4 background_color;
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
	Window _window;
	EngineConfig _engineConfig;
	gfx::Renderer _renderer;
	Ref<Application> _application;

	FRIEND_TEST(EngineTest, ConfigWindowTest);

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
	static void _terminate();
};

} // namespace arch
