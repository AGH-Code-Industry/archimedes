#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <window.h>
#include <functional>

#define BINDED_PAIR_ARRAY_SIZE 30
#ifndef ARCHIMEDES_INPUT_HANDLER_H
#define ARCHIMEDES_INPUT_HANDLER_H

namespace arch{
    class InputHandler{
    public:
        InputHandler(GLFWwindow *_window);
        void bindKey(int keyCode, std::function<void()> func); 
        void bindAll();       

    private:
        static GLFWwindow *_window;
        static std::unordered_map<int, std::function<void()>> bindedTable;
        static void closeWindow();
        static void maximizeWindow();
        static void handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
    };
}

#endif //ARCHIMEDES_INPUT_HANDLER_H