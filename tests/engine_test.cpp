//
// Created by tosiek on 22.02.23.
//

#include <gtest/gtest.h>
#include <engine.h>

namespace arch {
    TEST(EngineTest, CreatingEngine){
        arch::Engine engine;

        EXPECT_TRUE(&engine);
        engine.start();
    }

    TEST(EngineTest, ConfigWindowTest){
        arch::Engine engine;

        engine.initialize();

        int width, height;

        glfwGetWindowSize(engine._window.get(), &width, &height);

        engine.terminate();

        EXPECT_EQ(width, 1920);
        EXPECT_EQ(height, 1080);
    }
}
