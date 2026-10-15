#pragma once
#include <limits>
#include <random>
#include <vector>

#include <archimedes/Math.h>
#include <archimedes/algorithms/PerlinNoiseConfig.h>

namespace arch::algo {

/// @brief Perlin Noise generator for 1D input
/// The output function is continuous, yet looks somewhat randomised
/// It has many uses, like in procedural terrain/texture generation
class PerlinNoise1D {
	std::mt19937 _rng;
	std::uniform_real_distribution<f32> _gradientsDistribution{ -1.0f, 1.0f };
	std::vector<f32> _gradients;
	std::vector<f32> _offsets;
	u32 _nodesSize;
	i32 _seed;

	PerlinNoiseConfig _config{};

	i32 _generateSeed();
	void _generateGradients();
	void _generateOffsets(f32 minOffset, f32 maxOffset);
	f32 _generateOctave(f32 x) const;

public:
	// Rebuild the generator to initialize a new noise field
	// This generates the underlying gradients vector
	void build(u32 nodesSize);
	void build(u32 nodesSize, i32 seed);

	// These overloads generate a random offset for each node in the requested range,
	// modifying the range of possible output values
	void build(u32 nodesSize, f32 minOffset, f32 maxOffset);
	void build(u32 nodesSize, f32 minOffset, f32 maxOffset, i32 seed);

	/// @brief Constructor
	PerlinNoise1D() = default;

	// Setters
	PerlinNoise1D& setOctaves(u32 octaves);
	PerlinNoise1D& setBaseAmplitude(f32 baseAmplitude);
	PerlinNoise1D& setBaseFrequency(f32 baseFrequency);
	PerlinNoise1D& setAmplitudeFactor(f32 amplitudeFactor);
	PerlinNoise1D& setFrequencyFactor(f32 frequencyFactor);
	PerlinNoise1D& setMinResult(f32 minResult);
	PerlinNoise1D& setMaxResult(f32 maxResult);

	/// @brief Generates a Perlin Noise value for the given x coordinate
	f32 generate(f32 x) const;

	/// @brief Gets the current seed, which can reproduce the noise when used with the same settings
	i32 getSeed() const;

	/// Boundary getters
	f32 getMinResult() const;
	f32 getMaxResult() const;
};
} // namespace arch::algo
