#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <functional>

namespace arch{
    class InputHandler{
    public:
        static InputHandler& get();

        /**
         * Sets InputHandler to specific window and initializes key-function binds.
        */
        void initialize(GLFWwindow* window);       

    private:
        InputHandler();
        static InputHandler *instance;
        GLFWwindow *_window;
        static std::unordered_map<int, std::function<void()>> key_to_function_table;

        /**
         * Binds specific key code to a function.
        */
        void bind_key(int keyCode, std::function<void()> func); 

        /**
         * Callback function that runs when key is pressed or released.
        */
        static void handle_key_event(GLFWwindow *window, int key, int scancode, int action, int mods);
    };
}