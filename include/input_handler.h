#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <functional>

namespace arch{
    class InputHandler{
    public:
        InputHandler(const InputHandler&) = delete;
        static InputHandler& get();
        void setCurrentWindow(GLFWwindow* _window);       

    private:
        InputHandler();
        static InputHandler inputHandlerInstance;
        static GLFWwindow *_window;
        static std::unordered_map<int, std::function<void()>> bindedTable;
        void bindKey(int keyCode, std::function<void()> func); 
        void bindAll();
        static void closeWindow();
        static void maximizeWindow();
        static void iconifyWindow();
        static void restoreWindow();
        static void handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
    };
}