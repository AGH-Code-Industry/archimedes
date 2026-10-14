#pragma once
#include <archimedes/Math.h>

namespace arch::algorithms {

/// @brief Params used by Perlin Noise generators
struct PerlinNoiseConfig {
	/// @brief Number of periodic noise functions (octaves) that are added to get the output
	u32 octaves = 6;

	/// @brief Amplitude of the first octave
	f32 baseAmplitude = 0.6f;

	/// @brief Frequency of the first octave
	f32 baseFrequency = 0.005f;

	/// @brief Each subsequent octave's amplitude is multiplied by this number
	f32 amplitudeFactor = 0.5f;

	/// @brief Each subsequent octave's frequency is multiplied by this number
	f32 frequencyFactor = 2.0f;

	/// @brief Lower bound applied to the generated value
	f32 minResult = std::numeric_limits<f32>::lowest();

	/// @brief Upper bound applied to the generated value
	f32 maxResult = std::numeric_limits<f32>::max();
};
} // namespace arch::algorithms
