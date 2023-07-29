#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <window.h>

#ifndef ARCHIMEDES_INPUT_HANDLER_H
#define ARCHIMEDES_INPUT_HANDLER_H

namespace arch{
    class InputHandler{
    public:
        InputHandler(GLFWwindow *_window);        

    private:
        GLFWwindow *_window;

        static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    };
}

#endif //ARCHIMEDES_INPUT_HANDLER_H