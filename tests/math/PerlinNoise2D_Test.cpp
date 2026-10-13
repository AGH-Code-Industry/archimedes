#include <archimedes/Math.h>
#include <archimedes/Algorithms.h>
#include <gtest/gtest.h>

namespace math = arch::math;
namespace algorithms = arch::algorithms;

void assertValueInRange(math::f32 generated, math::f32 x, math::f32 y) {
	SCOPED_TRACE(
		::testing::Message() << std::format("x = {}, y = {}, generated = {}, seed = {}", x, y, generated, algorithms::PerlinNoise1D::getSeed())
	);
	ASSERT_FALSE(std::isnan(generated));
	ASSERT_GT(generated, algorithms::PerlinNoise2D::minResult);
	ASSERT_LT(generated, algorithms::PerlinNoise2D::maxResult);
}

TEST(MathTest, PerlinNoise2DGenerateBeforeBuildTest) {
	ASSERT_THROW(algorithms::PerlinNoise2D::generate(0.5f, 0.5f), algorithms::AlgorithmException);
}

TEST(MathTest, PerlinNoise2DRangeTest) {
	algorithms::PerlinNoise2D::build(100, 0.0f, 0.1f);
	algorithms::PerlinNoise2D::minResult = -1.0f;
	algorithms::PerlinNoise2D::maxResult = 1.0f;

	for (math::i32 i = 0; i < 100; i++) {
		for (math::i32 j = 0; j < 100; j++) {
			math::f32 x = (math::f32)i / 100.0f;
			math::f32 y = (math::f32)j / 100.0f;
			math::f32 generated = algorithms::PerlinNoise2D::generate(x, y);
			assertValueInRange(generated, x, y);
		}
	}
}

TEST(MathTest, PerlinNoise2DSeedTest) {
	algorithms::PerlinNoise2D::build(100, 0.0f, 0.1f);
	math::f32 firstValue = algorithms::PerlinNoise2D::generate(0.5f, 0.5f);
	math::i32 seed = algorithms::PerlinNoise2D::getSeed();
	algorithms::PerlinNoise2D::build(100, 0.0f, 0.1f, seed);
	math::f32 secondValue = algorithms::PerlinNoise2D::generate(0.5f, 0.5f);
	ASSERT_EQ(firstValue, secondValue);
}

TEST(MathTest, PerlinNoise2DWrongBuildTest) {
	ASSERT_THROW(algorithms::PerlinNoise2D::build(0, 0.0f, 0.1f), algorithms::AlgorithmException);
	ASSERT_THROW(algorithms::PerlinNoise2D::build(100, 0.1f, 0.0f), algorithms::AlgorithmException);
}

TEST(MathTest, PerlinNoise2DNegativeCoordinatesTest) {
	algorithms::PerlinNoise2D::build(100, 0.0f, 0.1f);
	algorithms::PerlinNoise2D::minResult = -1.0f;
	algorithms::PerlinNoise2D::maxResult = 1.0f;

	for (math::i32 i = -50; i < 50; i++) {
		for (math::i32 j = -50; j < 50; j++) {
			math::f32 x = (math::f32)i / 10.0f;
			math::f32 y = (math::f32)j / 10.0f;
			math::f32 generated = algorithms::PerlinNoise2D::generate(x, y);
			assertValueInRange(generated, x, y);
		}
	}
}

TEST(MathTest, PerlinNoise2DNoOffsetsTest) {
	algorithms::PerlinNoise2D::build(100);
	algorithms::PerlinNoise2D::minResult = -1.0f;
	algorithms::PerlinNoise2D::maxResult = 1.0f;

	for (math::i32 i = -50; i < 50; i++) {
		for (math::i32 j = -50; j < 50; j++) {
			math::f32 x = (math::f32)i / 10.0f;
			math::f32 y = (math::f32)j / 10.0f;
			math::f32 generated = algorithms::PerlinNoise2D::generate(x, y);
			assertValueInRange(generated, x, y);
		}
	}
}

TEST(MathTest, PerlinNoise2DClearsOffsetsOnRebuildTest) {
	constexpr math::i32 seed = 12'345;

	algorithms::PerlinNoise2D::build(10, 1.0f, 1.0f, seed);
	ASSERT_NE(algorithms::PerlinNoise2D::generate(0.0f, 0.0f), 0.0f);

	algorithms::PerlinNoise2D::build(10, seed);
	EXPECT_FLOAT_EQ(algorithms::PerlinNoise2D::generate(0.0f, 0.0f), 0.0f);
}
