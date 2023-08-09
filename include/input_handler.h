#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <functional>

namespace arch{
    class InputHandler{
    public:
        InputHandler(const InputHandler&) = delete;
        InputHandler(const InputHandler&&) = delete;
        static InputHandler& get();
        void set_current_window(GLFWwindow* window);       

    private:
        InputHandler();
        static InputHandler input_handler_instance;
        static GLFWwindow *_window;
        static std::unordered_map<int, std::function<void()>> binded_table;
        void bind_key(int keyCode, std::function<void()> func); 
        void bind_all();
        static void close_window();
        static void maximize_window();
        static void iconify_window();
        static void restore_window();
        static void handle_key_event(GLFWwindow *window, int key, int scancode, int action, int mods);
    };
}