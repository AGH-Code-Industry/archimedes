//
// Created by tosiek on 22.02.23.
//

#include <gtest/gtest.h>
#include <engine.h>

TEST(EngineTest, CreatingEngine){
    arch::Engine engine;

    EXPECT_TRUE(&engine);
}
