#pragma once
#include <archimedes/math/Math.h>
#include <random>
#include <vector>
#include <limits>


namespace arch::math {

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
class PerlinNoise2D
{
    inline static std::mt19937 _rng;
    inline static std::vector<u32> _permutation;
    inline static std::vector<std::vector<f32>> _offsets;
    inline static u32 _gridSize;
    inline static i32 _seed;

    inline static const std::array<float2, 4> _constantVectors = {
        float2(1.0, 1.0),
        float2(-1.0, 1.0),
        float2(-1.0, -1.0),
        float2(1.0, -1.0),
    };

    static i32 _generateSeed();
    static u32 _getHash(u32 x, u32 y);
    static void _generatePermutation();
    static void _generateOffsets(f32 minOffset, f32 maxOffset);
    static f32 _generateOctave(f32 x, f32 y);

    static GridCell _getGridCell(f32 x, f32 y);
    static f32 _getCornerContribution(const GridCell& cell, u32 offsetX, u32 offsetY);
    static f32 _interpolateCell(f32 bottomLeft, f32 bottomRight, f32 topLeft, f32 topRight, f32 localX, f32 localY);

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
    // This generates the underlying permutation table
    static void build(u32 gridSize);
    static void build(u32 gridSize, i32 seed);

    // These overloads generate a random offset for each grid point in the requested range,
    // modifying the range of possible output values
    static void build(u32 gridSize, f32 minOffset, f32 maxOffset);
    static void build(u32 gridSize, f32 minOffset, f32 maxOffset, i32 seed);

    /// @brief Generates a Perlin Noise value for the given x and y coordinates
    static f32 generate(f32 x, f32 y);

    /// @brief Gets the current seed, which can reproduce the noise when used with the same settings
    static i32 getSeed();
};
}
