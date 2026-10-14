#include <archimedes/Algorithms.h>
#include <archimedes/Math.h>
#include <gtest/gtest.h>

namespace math = arch::math;
namespace algorithms = arch::algorithms;

static void assertValueInRange(const algorithms::PerlinNoise1D& noise, math::f32 generated, math::f32 x) {
	SCOPED_TRACE(
		::testing::Message() << std::format("x = {}, generated = {}, seed = {}", x, generated, noise.getSeed())
	);

	ASSERT_FALSE(std::isnan(generated));
	ASSERT_GT(generated, noise.getMinResult());
	ASSERT_LT(generated, noise.getMaxResult());
}

TEST(AlgorithmsTest, PerlinNoise1DGenerateBeforeBuildTest) {
	algorithms::PerlinNoise1D noise;

	ASSERT_THROW(noise.generate(0.5f), algorithms::AlgorithmException);
}

TEST(AlgorithmsTest, PerlinNoise1DRangeTest) {
	algorithms::PerlinNoise1D noise;

	noise.build(100, 0.0f, 0.1f);
	noise.setMinResult(-1.0f).setMaxResult(1.0f);

	for (math::i32 i = 0; i < 10'000; i++) {
		math::f32 x = (math::f32)i / 100.0f;
		math::f32 generated = noise.generate(x);
		assertValueInRange(noise, generated, x);
	}
}

TEST(AlgorithmsTest, PerlinNoise1DSeedTest) {
	algorithms::PerlinNoise1D noise;

	noise.build(100, 0.0f, 0.1f);
	math::f32 firstValue = noise.generate(0.5f);
	math::i32 seed = noise.getSeed();
	noise.build(100, 0.0f, 0.1f, seed);
	math::f32 secondValue = noise.generate(0.5f);
	ASSERT_EQ(firstValue, secondValue);
}

TEST(AlgorithmsTest, PerlinNoise1DWrongBuildTest) {
	algorithms::PerlinNoise1D noise;

	ASSERT_THROW(noise.build(0, 0.0f, 0.1f), algorithms::AlgorithmException);
	ASSERT_THROW(noise.build(100, 0.1f, 0.0f), algorithms::AlgorithmException);
}

TEST(AlgorithmsTest, PerlinNoise1DNegativeCoordinatesTest) {
	algorithms::PerlinNoise1D noise;

	noise.build(100, 0.0f, 0.1f);
	noise.setMinResult(-1.0f).setMaxResult(1.0f);

	for (math::i32 i = -500; i < 500; i++) {
		math::f32 x = (math::f32)i / 10.0f;
		math::f32 generated = noise.generate(x);
		assertValueInRange(noise, generated, x);
	}
}

TEST(AlgorithmsTest, PerlinNoise1DNoOffsetsTest) {
	algorithms::PerlinNoise1D noise;

	noise.build(100);
	noise.setMinResult(-1.0f).setMaxResult(1.0f);

	for (math::i32 i = -500; i < 500; i++) {
		math::f32 x = (math::f32)i / 10.0f;
		math::f32 generated = noise.generate(x);
		assertValueInRange(noise, generated, x);
	}
}

TEST(AlgorithmsTest, PerlinNoise1DClearsOffsetsOnRebuildTest) {
	algorithms::PerlinNoise1D noise;
	constexpr math::i32 seed = 12'345;

	noise.build(10, 1.0f, 1.0f, seed);
	ASSERT_NE(noise.generate(0.0f), 0.0f);

	noise.build(10, seed);
	EXPECT_FLOAT_EQ(noise.generate(0.0f), 0.0f);
}
