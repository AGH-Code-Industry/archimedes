#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <window.h>

#define BINDED_PAIR_ARRAY_SIZE 30
#ifndef ARCHIMEDES_INPUT_HANDLER_H
#define ARCHIMEDES_INPUT_HANDLER_H

namespace arch{
    class InputHandler{
    public:
        InputHandler(GLFWwindow *_window);
        void bindKey(int keyCode, void (*func)());        
        struct bindedPair {
            int keyCode;
            void (*func_pointer)();
        };
        static bindedPair bindedPairArray[BINDED_PAIR_ARRAY_SIZE];
    private:
        GLFWwindow *_window;
        
        static void handleKeyEvent(GLFWwindow *window, int key, int scancode, int action, int mods);
        void initializeBindedPairArray();
    };
}

#endif //ARCHIMEDES_INPUT_HANDLER_H