#pragma once
#include <limits>
#include <random>
#include <vector>

#include <archimedes/Math.h>
#include <archimedes/algorithms/PerlinNoiseConfig.h>

namespace arch::algorithms {

/// @brief Helper structure used to calculate gradients for 2D Perlin Noise
struct GridCell {
	u32 gridX;
	u32 gridY;
	f32 localX;
	f32 localY;
};

/// @brief Perlin Noise generator for 2D input
/// The output function is continuous, yet looks somewhat randomised
/// It has many uses, like in procedural terrain/texture generation
class PerlinNoise2D {
	std::mt19937 _rng;
	std::vector<u32> _permutation;
	std::vector<std::vector<f32>> _offsets;
	u32 _gridSize;
	i32 _seed;

	const std::array<float2, 4> _constantVectors = {
		float2(1.0, 1.0),
		float2(-1.0, 1.0),
		float2(-1.0, -1.0),
		float2(1.0, -1.0),
	};

	PerlinNoiseConfig _config{};

	i32 _generateSeed();
	u32 _getHash(u32 x, u32 y) const;
	void _generatePermutation();
	void _generateOffsets(f32 minOffset, f32 maxOffset);
	f32 _generateOctave(f32 x, f32 y) const;
	GridCell _getGridCell(f32 x, f32 y) const;
	f32 _getCornerContribution(const GridCell& cell, u32 offsetX, u32 offsetY) const;
	f32 _interpolateCell(f32 bottomLeft, f32 bottomRight, f32 topLeft, f32 topRight, f32 localX, f32 localY) const;

public:
	// Rebuild the generator to initialize a new noise field
	// This generates the underlying permutation table
	void build(u32 gridSize);
	void build(u32 gridSize, i32 seed);

	// These overloads generate a random offset for each grid point in the requested range,
	// modifying the range of possible output values
	void build(u32 gridSize, f32 minOffset, f32 maxOffset);
	void build(u32 gridSize, f32 minOffset, f32 maxOffset, i32 seed);

	/// @brief Constructor
	PerlinNoise2D() = default;

	// Setters
	PerlinNoise2D& setOctaves(u32 octaves);
	PerlinNoise2D& setBaseAmplitude(f32 baseAmplitude);
	PerlinNoise2D& setBaseFrequency(f32 baseFrequency);
	PerlinNoise2D& setAmplitudeFactor(f32 amplitudeFactor);
	PerlinNoise2D& setFrequencyFactor(f32 frequencyFactor);
	PerlinNoise2D& setMinResult(f32 minResult);
	PerlinNoise2D& setMaxResult(f32 maxResult);

	/// @brief Generates a Perlin Noise value for the given x and y coordinates
	f32 generate(f32 x, f32 y) const;

	/// @brief Gets the current seed, which can reproduce the noise when used with the same settings
	i32 getSeed() const;

	/// Boundary getters
	f32 getMinResult() const;
	f32 getMaxResult() const;
};
} // namespace arch::algorithms
