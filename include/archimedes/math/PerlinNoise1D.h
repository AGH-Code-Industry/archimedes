#pragma once
#include <archimedes/math/Math.h>
#include <random>
#include <vector>
#include <limits>


namespace arch::math {

/// @brief Perlin Noise generator for 1D input
/// The output function is continuous, yet looks somewhat randomised
/// It has many uses, like in procedural terrain/texture generation
class PerlinNoise1D
{
    inline static std::mt19937 _rng;
    inline static auto _gradientsDistribution = std::uniform_real_distribution<f32>(-1.0f, 1.0f);
    inline static std::vector<f32> _gradients;
    inline static std::vector<f32> _offsets;
    inline static u32 _nodesSize;
    inline static i32 _seed;

    static i32 _generateSeed();
    static void _generateGradients();
    static void _generateOffsets(f32 minOffset, f32 maxOffset);
    static f32 _generateOctave(f32 x);

public:
    /// @brief Number of periodic noise functions (octaves) that are added to get the output
    inline static u32 octaves = 6;

    /// @brief Amplitude of the first octave
    inline static f32 baseAmplitude = 0.6f;

    /// @brief Frequency of the first octave
    inline static f32 baseFrequency = 0.005f;

    /// @brief Each subsequent octave's amplitude is multiplied by this number
    inline static f32 amplitudeFactor = 0.5f;

    /// @brief Each subsequent octave's frequency is multiplied by this number
    inline static f32 frequencyFactor = 2.0f;

    /// @brief Lower bound applied to the generated value
    inline static f32 minResult = std::numeric_limits<f32>::lowest();

    /// @brief Upper bound applied to the generated value
    inline static f32 maxResult = std::numeric_limits<f32>::max();

    // Rebuild the generator to initialize a new noise field
    // This generates the underlying gradients vector
    static void build(u32 nodesSize);
    static void build(u32 nodesSize, i32 seed);

    // These overloads generate a random offset for each node in the requested range,
    // modifying the range of possible output values
    static void build(u32 nodesSize, f32 minOffset, f32 maxOffset);
    static void build(u32 nodesSize, f32 minOffset, f32 maxOffset, i32 seed);

    /// @brief Generates a Perlin Noise value for the given x coordinate
    static f32 generate(f32 x);

    /// @brief Gets the current seed, which can reproduce the noise when used with the same settings
    static i32 getSeed();
};
}
