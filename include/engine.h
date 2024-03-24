#ifndef ARCHIMEDES_ENGINE_H
#define ARCHIMEDES_ENGINE_H

#include <gfx/Renderer.h>
#include <gtest/gtest_prod.h>
#include <window.h>

#include <fstream>
#include <string>

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
    gfx::Renderer _renderer;

    FRIEND_TEST(EngineTest, ConfigWindowTest);

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
}

#endif //ARCHIMEDES_ENGINE_H
