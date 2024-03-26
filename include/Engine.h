#pragma once

#include <fstream>
#include <string>

#include "gfx/Renderer.h"
#include "window.h"

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
	Engine(const EngineConfig& config);
	~Engine();

	/**
	 * Only function visible for user.
	 */
	void start();

private:
	Window _window;
	EngineConfig _engine_config;
	gfx::Renderer _renderer;

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
