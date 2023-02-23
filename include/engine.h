//
// Created by tosiek on 22.02.23.
//

#ifndef ARCHIMEDES_ENGINE_H
#define ARCHIMEDES_ENGINE_H

#include <window.h>

namespace arch {
    /**
     * Main class of Archimedes project.
     */
    class Engine {
    public:
        /**
         * Only function visible for user.
         */
        void start();

        Engine(): _background_color(0.0f, 0.0f, 0.0f, 1.0f){}

    private:
        Window _window;
        glm::vec4 _background_color;

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
        void terminate();

        /**
         * Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
         */
        void process_input();
    };
}

#endif //ARCHIMEDES_ENGINE_H
