#include <archimedes/Algorithms.h>
#include <archimedes/Math.h>
#include <gtest/gtest.h>

namespace math = arch::math;
namespace algorithms = arch::algo;

void assertValueInRange(const algorithms::PerlinNoise2D& noise, math::f32 generated, math::f32 x, math::f32 y) {
	SCOPED_TRACE(
		::testing::Message()
		<< std::format("x = {}, y = {}, generated = {}, seed = {}", x, y, generated, noise.getSeed())
	);
	ASSERT_FALSE(std::isnan(generated));
	ASSERT_GT(generated, noise.getMinResult());
	ASSERT_LT(generated, noise.getMaxResult());
}

TEST(AlgorithmsTest, PerlinNoise2DGenerateBeforeBuildTest) {
	algorithms::PerlinNoise2D noise;

	ASSERT_THROW(noise.generate(0.5f, 0.5f), algorithms::AlgorithmException);
}

TEST(AlgorithmsTest, PerlinNoise2DRangeTest) {
	algorithms::PerlinNoise2D noise;

	noise.build(100, 0.0f, 0.1f);
	noise.setMinResult(-1.0f).setMaxResult(1.0f);

	for (math::i32 i = 0; i < 100; i++) {
		for (math::i32 j = 0; j < 100; j++) {
			math::f32 x = (math::f32)i / 100.0f;
			math::f32 y = (math::f32)j / 100.0f;
			math::f32 generated = noise.generate(x, y);
			assertValueInRange(noise, generated, x, y);
		}
	}
}

TEST(AlgorithmsTest, PerlinNoise2DSeedTest) {
	algorithms::PerlinNoise2D noise;

	noise.build(100, 0.0f, 0.1f);
	math::f32 firstValue = noise.generate(0.5f, 0.5f);
	math::i32 seed = noise.getSeed();
	noise.build(100, 0.0f, 0.1f, seed);
	math::f32 secondValue = noise.generate(0.5f, 0.5f);
	ASSERT_EQ(firstValue, secondValue);
}

TEST(AlgorithmsTest, PerlinNoise2DWrongBuildTest) {
	algorithms::PerlinNoise2D noise;

	ASSERT_THROW(noise.build(0, 0.0f, 0.1f), algorithms::AlgorithmException);
	ASSERT_THROW(noise.build(100, 0.1f, 0.0f), algorithms::AlgorithmException);
}

TEST(AlgorithmsTest, PerlinNoise2DNegativeCoordinatesTest) {
	algorithms::PerlinNoise2D noise;

	noise.build(100, 0.0f, 0.1f);
	noise.setMinResult(-1.0f).setMaxResult(1.0f);

	for (math::i32 i = -50; i < 50; i++) {
		for (math::i32 j = -50; j < 50; j++) {
			math::f32 x = (math::f32)i / 10.0f;
			math::f32 y = (math::f32)j / 10.0f;
			math::f32 generated = noise.generate(x, y);
			assertValueInRange(noise, generated, x, y);
		}
	}
}

TEST(AlgorithmsTest, PerlinNoise2DNoOffsetsTest) {
	algorithms::PerlinNoise2D noise;

	noise.build(100);
	noise.setMinResult(-1.0f).setMaxResult(1.0f);

	for (math::i32 i = -50; i < 50; i++) {
		for (math::i32 j = -50; j < 50; j++) {
			math::f32 x = (math::f32)i / 10.0f;
			math::f32 y = (math::f32)j / 10.0f;
			math::f32 generated = noise.generate(x, y);
			assertValueInRange(noise, generated, x, y);
		}
	}
}

TEST(AlgorithmsTest, PerlinNoise2DClearsOffsetsOnRebuildTest) {
	algorithms::PerlinNoise2D noise;
	constexpr math::i32 seed = 12'345;

	noise.build(10, 1.0f, 1.0f, seed);
	ASSERT_NE(noise.generate(0.0f, 0.0f), 0.0f);

	noise.build(10, seed);
	EXPECT_FLOAT_EQ(noise.generate(0.0f, 0.0f), 0.0f);
}
