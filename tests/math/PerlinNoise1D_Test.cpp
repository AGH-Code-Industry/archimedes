#include <archimedes/Math.h>
#include <gtest/gtest.h>

namespace math = arch::math;

static void assertValueInRange(math::f32 generated, math::f32 x) {
    SCOPED_TRACE(
        ::testing::Message()
            << "x = " << x
            << ", generated = " << generated
            << ", seed = " << math::PerlinNoise1D::getSeed()
    );

    ASSERT_FALSE(std::isnan(generated));
    ASSERT_GT(generated, math::PerlinNoise1D::minResult);
    ASSERT_LT(generated, math::PerlinNoise1D::maxResult);
}

TEST(MathTest, PerlinNoise1DGenerateBeforeBuildTest) {
    ASSERT_THROW(math::PerlinNoise1D::generate(0.5f), math::MathException);
}

TEST(MathTest, PerlinNoise1DRangeTest) {
    math::PerlinNoise1D::build(100, 0.0f, 0.1f);
    math::PerlinNoise1D::minResult = -1.0f;
    math::PerlinNoise1D::maxResult = 1.0f;

    for(math::i32 i=0; i<10000; i++){
        math::f32 x = (math::f32) i / 100.0f;
        math::f32 generated = math::PerlinNoise1D::generate(x);
        assertValueInRange(generated, x);
    }
}

TEST(MathTest, PerlinNoise1DSeedTest) {
    math::PerlinNoise1D::build(100, 0.0f, 0.1f);
    math::f32 firstValue = math::PerlinNoise1D::generate(0.5f);
    math::i32 seed = math::PerlinNoise1D::getSeed();
    math::PerlinNoise1D::build(100, 0.0f, 0.1f, seed);
    math::f32 secondValue = math::PerlinNoise1D::generate(0.5f);
    ASSERT_EQ(firstValue, secondValue);
}
TEST(MathTest, PerlinNoise1DWrongBuildTest) {
    ASSERT_THROW(math::PerlinNoise1D::build(0, 0.0f, 0.1f), math::MathException);
    ASSERT_THROW(math::PerlinNoise1D::build(100, 0.1f, 0.0f), math::MathException);
}

TEST(MathTest, PerlinNoise1DNegativeCoordinatesTest) {
    math::PerlinNoise1D::build(100, 0.0f, 0.1f);
    math::PerlinNoise1D::minResult = -1.0f;
    math::PerlinNoise1D::maxResult = 1.0f;

    for(math::i32 i=-500; i<500; i++){
        math::f32 x = (math::f32) i / 10.0f;
        math::f32 generated = math::PerlinNoise1D::generate(x);
        assertValueInRange(generated, x);
    }
}

TEST(MathTest, PerlinNoise1DNoOffsetsTest) {
    math::PerlinNoise1D::build(100);
    math::PerlinNoise1D::minResult = -1.0f;
    math::PerlinNoise1D::maxResult = 1.0f;

    for(math::i32 i=-500; i<500; i++){
        math::f32 x = (math::f32) i / 10.0f;
        math::f32 generated = math::PerlinNoise1D::generate(x);
        assertValueInRange(generated, x);
    }
}