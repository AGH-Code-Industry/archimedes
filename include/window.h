//
// Created by tosiek on 22.02.23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

#ifndef ARCHIMEDES_WINDOW_H
#define ARCHIMEDES_WINDOW_H

namespace arch {
    class Window {
    public:
        /**
         * Constructor.
         * @param width Window's width.
         * @param height Window's height.
         * @param name Window's name.
         * @param monitor The monitor to use for fullscreen mode.
         * @param share The window whose context to share resources with, or NULL to not share resources.
         */
        Window(int width, int height, const std::string& name, GLFWmonitor* monitor, Window &share);
        /**
         * Constructor.
         * @param width Window's width.
         * @param height Window's height.
         * @param name Window's name.
         * @param monitor The monitor to use for fullscreen mode.
         */
        Window(int width, int height, const std::string& name, GLFWmonitor* monitor = nullptr);

        /**
         * @return Window's object.
         */
        GLFWwindow* get();

    private:
        GLFWwindow* _window;

    };
}

#endif //ARCHIMEDES_WINDOW_H
