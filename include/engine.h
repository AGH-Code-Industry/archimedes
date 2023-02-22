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

    private:
        Window _window;

        /**
         * Responsible for drawing game on the screen.
         */
        void main_loop();

        /**
         * Initialize glfw in engine and creates window.
         */
        void initialize();
    };
}

#endif //ARCHIMEDES_ENGINE_H
