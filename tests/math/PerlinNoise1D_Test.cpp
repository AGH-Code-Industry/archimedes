#include <archimedes/Math.h>
#include <archimedes/Algorithms.h>
#include <gtest/gtest.h>

namespace math = arch::math;
namespace algorithms = arch::algorithms;

static void assertValueInRange(math::f32 generated, math::f32 x) {
	SCOPED_TRACE(
		::testing::Message() << std::format("x = {}, generated = {}, seed = {}", x, generated, algorithms::PerlinNoise1D::getSeed())
	);

	ASSERT_FALSE(std::isnan(generated));
	ASSERT_GT(generated, algorithms::PerlinNoise1D::minResult);
	ASSERT_LT(generated, algorithms::PerlinNoise1D::maxResult);
}

TEST(MathTest, PerlinNoise1DGenerateBeforeBuildTest) {
	ASSERT_THROW(algorithms::PerlinNoise1D::generate(0.5f), algorithms::AlgorithmException);
}

TEST(MathTest, PerlinNoise1DRangeTest) {
	algorithms::PerlinNoise1D::build(100, 0.0f, 0.1f);
	algorithms::PerlinNoise1D::minResult = -1.0f;
	algorithms::PerlinNoise1D::maxResult = 1.0f;

	for (math::i32 i = 0; i < 10'000; i++) {
		math::f32 x = (math::f32)i / 100.0f;
		math::f32 generated = algorithms::PerlinNoise1D::generate(x);
		assertValueInRange(generated, x);
	}
}

TEST(MathTest, PerlinNoise1DSeedTest) {
	algorithms::PerlinNoise1D::build(100, 0.0f, 0.1f);
	math::f32 firstValue = algorithms::PerlinNoise1D::generate(0.5f);
	math::i32 seed = algorithms::PerlinNoise1D::getSeed();
	algorithms::PerlinNoise1D::build(100, 0.0f, 0.1f, seed);
	math::f32 secondValue = algorithms::PerlinNoise1D::generate(0.5f);
	ASSERT_EQ(firstValue, secondValue);
}

TEST(MathTest, PerlinNoise1DWrongBuildTest) {
	ASSERT_THROW(algorithms::PerlinNoise1D::build(0, 0.0f, 0.1f), algorithms::AlgorithmException);
	ASSERT_THROW(algorithms::PerlinNoise1D::build(100, 0.1f, 0.0f), algorithms::AlgorithmException);
}

TEST(MathTest, PerlinNoise1DNegativeCoordinatesTest) {
	algorithms::PerlinNoise1D::build(100, 0.0f, 0.1f);
	algorithms::PerlinNoise1D::minResult = -1.0f;
	algorithms::PerlinNoise1D::maxResult = 1.0f;

	for (math::i32 i = -500; i < 500; i++) {
		math::f32 x = (math::f32)i / 10.0f;
		math::f32 generated = algorithms::PerlinNoise1D::generate(x);
		assertValueInRange(generated, x);
	}
}

TEST(MathTest, PerlinNoise1DNoOffsetsTest) {
	algorithms::PerlinNoise1D::build(100);
	algorithms::PerlinNoise1D::minResult = -1.0f;
	algorithms::PerlinNoise1D::maxResult = 1.0f;

	for (math::i32 i = -500; i < 500; i++) {
		math::f32 x = (math::f32)i / 10.0f;
		math::f32 generated = algorithms::PerlinNoise1D::generate(x);
		assertValueInRange(generated, x);
	}
}

TEST(MathTest, PerlinNoise1DClearsOffsetsOnRebuildTest) {
	constexpr math::i32 seed = 12'345;

	algorithms::PerlinNoise1D::build(10, 1.0f, 1.0f, seed);
	ASSERT_NE(algorithms::PerlinNoise1D::generate(0.0f), 0.0f);

	algorithms::PerlinNoise1D::build(10, seed);
	EXPECT_FLOAT_EQ(algorithms::PerlinNoise1D::generate(0.0f), 0.0f);
}
