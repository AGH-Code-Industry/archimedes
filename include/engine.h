//
// Created by tosiek on 22.02.23.
//

#ifndef ARCHIMEDES_ENGINE_H
#define ARCHIMEDES_ENGINE_H

#include <window.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <exceptions/config_exception.h>
//#include <gtest/gtest_prod.h>

namespace arch {
    /**
     * Main class of Archimedes project.
     */
    class Engine {
    public:
        struct EngineConfig {
            int window_width;
            int window_height;
            std::string window_title;
            glm::vec4 background_color;
        };
        Engine();
        ~Engine();

        /**
         * Only function visible for user.
         */
        void start();

    private:
        Window _window;
        EngineConfig _engine_config;

        //FRIEND_TEST(EngineTest, ConfigWindowTest);

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

        /**
         * Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
         */
        void process_input();

        /**
         * Loads configuration files for engine.
         */
        void load_configuration();
    };
}

#endif //ARCHIMEDES_ENGINE_H
