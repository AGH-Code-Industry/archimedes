#include <archimedes/Mmath.h>
#include <gtest/gtest.h>

namespace math = arch::math;

TEST(MathTest, PerlinNoise2DRangeTest) {
    math::PerlinNoise2D::build(100, 0.0f, 0.1f);

    for(math::i32 i=0; i<100; i++){
        for(math::i32 j=0; j<100; j++){
            math::f32 x = (math::f32) i / 100.0f;
            math::f32 y = (math::f32) j / 100.0f;
            math::f32 generated = math::PerlinNoise2D::generate(x, y);
            ASSERT_FALSE(std::isnan(generated));
            ASSERT_TRUE(math::PerlinNoise2D::minResult < generated);
            ASSERT_TRUE(generated < math::PerlinNoise2D::maxResult);
        }
    }
}
