#ifndef ARCHIMEDES_ENGINE_H
#define ARCHIMEDES_ENGINE_H

#include <exceptions/config_exception.h>
#include <gtest/gtest_prod.h>
#include <window.h>

#include <fstream>
#include <string>

#include "resource/imgui_loader.h"

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
    Engine(EngineConfig config);
    ~Engine();

    /**
     * Only function visible for user.
     */
    void start();

private:
    Window _window;
    EngineConfig _engine_config;

	FRIEND_TEST(EngineTest, ConfigWindowTest);

#if ARCHIMEDES_DEBUG
	ImGuiLoader* _imgui = nullptr;
#endif

	/**
	 * Responsible for drawing game on the screen.
	 */
	void main_loop();

	/**
	 * Initialize glfw in engine and creates window.
	 */
	void initialize();

	/**
	 * Clearing all previously allocated GLFW and Engine resources.
	 */
	static void terminate();
};
}  // namespace arch

#endif	// ARCHIMEDES_ENGINE_H
