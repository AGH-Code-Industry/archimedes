//
// Created by tosiek on 22.02.23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

#include <exceptions/init_exception.h>
#include <glm/vec4.hpp>

#ifndef ARCHIMEDES_WINDOW_H
#define ARCHIMEDES_WINDOW_H

namespace arch {
    class Window {
    public:
        Window(const Window &other) = delete;
        /**
         * Constructor.
         * @param width Window's width.
         * @param height Window's height.
         * @param name Window's name.
         * @param monitor The pointer to the monitor to use for fullscreen mode.
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

        void clear(glm::vec4 color);
        void clear(float r, float g, float b, float a);
        void swap_buffers();
        void resize(int width, int height);
        void set_title(const std::string &title);

        bool should_close();
        Window& operator=(const Window& w) = delete;

    private:
        std::string _title;
        GLFWwindow* _window {};

        /**
         * Initializes window
         * @param width Window's width.
         * @param height Window's height.
         * @param name Window's name.
         * @param monitor The monitor to use for fullscreen mode.
         * @param window The window whose context to share resources with, or NULL to not share resources.
         */
        void initialize(int width, int height, const char* name, GLFWmonitor* monitor, GLFWwindow *window);

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    };
}

#endif //ARCHIMEDES_WINDOW_H
