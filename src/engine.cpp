//
// Created by tosiek on 22.02.23.
//

#include "engine.h"

using namespace arch;

void Engine::start() {
    if (!glfwInit())
        throw InitException();
}

void Engine::main_loop() {

}
