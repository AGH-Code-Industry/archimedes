#include <archimedes/Math.h>
#include <gtest/gtest.h>

namespace math = arch::math;

void assertValueInRange(math::f32 generated, math::f32 x, math::f32 y) {
    SCOPED_TRACE(
        ::testing::Message()
            << "x = " << x
            << ", y = " << y
            << ", generated = " << generated
            << ", seed = " << math::PerlinNoise2D::getSeed()
    );

    ASSERT_FALSE(std::isnan(generated));
    ASSERT_GT(generated, math::PerlinNoise2D::minResult);
    ASSERT_LT(generated, math::PerlinNoise2D::maxResult);
}

TEST(MathTest, PerlinNoise2DGenerateBeforeBuildTest) {
    ASSERT_THROW(math::PerlinNoise2D::generate(0.5f, 0.5f), math::MathException);
}

TEST(MathTest, PerlinNoise2DRangeTest) {
    math::PerlinNoise2D::build(100, 0.0f, 0.1f);
    math::PerlinNoise2D::minResult = -1.0f;
    math::PerlinNoise2D::maxResult = 1.0f;

    for(math::i32 i=0; i<100; i++){
        for(math::i32 j=0; j<100; j++){
            math::f32 x = (math::f32) i / 100.0f;
            math::f32 y = (math::f32) j / 100.0f;
            math::f32 generated = math::PerlinNoise2D::generate(x, y);
            assertValueInRange(generated, x, y);
        }
    }
}

TEST(MathTest, PerlinNoise2DSeedTest) {
    math::PerlinNoise2D::build(100, 0.0f, 0.1f);
    math::f32 firstValue = math::PerlinNoise2D::generate(0.5f, 0.5f);
    math::i32 seed = math::PerlinNoise2D::getSeed();
    math::PerlinNoise2D::build(100, 0.0f, 0.1f, seed);
    math::f32 secondValue = math::PerlinNoise2D::generate(0.5f, 0.5f);
    ASSERT_EQ(firstValue, secondValue);
}

TEST(MathTest, PerlinNoise2DWrongBuildTest) {
    ASSERT_THROW(math::PerlinNoise2D::build(0, 0.0f, 0.1f), math::MathException);
    ASSERT_THROW(math::PerlinNoise2D::build(100, 0.1f, 0.0f), math::MathException);
}

TEST(MathTest, PerlinNoise2DNegativeCoordinatesTest) {
    math::PerlinNoise2D::build(100, 0.0f, 0.1f);
    math::PerlinNoise2D::minResult = -1.0f;
    math::PerlinNoise2D::maxResult = 1.0f;

    for(math::i32 i=-50; i<50; i++){
        for(math::i32 j=-50; j<50; j++){
            math::f32 x = (math::f32) i / 10.0f;
            math::f32 y = (math::f32) j / 10.0f;
            math::f32 generated = math::PerlinNoise2D::generate(x, y);
            assertValueInRange(generated, x, y);
        }
    }
}

TEST(MathTest, PerlinNoise2DNoOffsetsTest) {
    math::PerlinNoise2D::build(100);
    math::PerlinNoise2D::minResult = -1.0f;
    math::PerlinNoise2D::maxResult = 1.0f;

    for(math::i32 i=-50; i<50; i++){
        for(math::i32 j=-50; j<50; j++){
            math::f32 x = (math::f32) i / 10.0f;
            math::f32 y = (math::f32) j / 10.0f;
            math::f32 generated = math::PerlinNoise2D::generate(x, y);
            assertValueInRange(generated, x, y);
        }
    }
}