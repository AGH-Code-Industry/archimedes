#pragma once
#include <archimedes/math/Math.h>
#include <random>
#include <vector>
#include <limits>


namespace arch::math {

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
    inline static u32 octaves = 6;
    inline static f32 baseAmplitude = 0.6f;
    inline static f32 baseFrequency = 0.005f;
    inline static f32 amplitudeFactor = 0.5f;
    inline static f32 frequencyFactor = 2.0f;
    inline static f32 minResult = std::numeric_limits<f32>::lowest();
    inline static f32 maxResult = std::numeric_limits<f32>::max();

    static void build(u32 nodesSize, f32 minOffset, f32 maxOffset);
    static void build(u32 nodesSize, f32 minOffset, f32 maxOffset, i32 seed);

    static void build(u32 nodesSize);
    static void build(u32 nodesSize, i32 seed);

    static f32 generate(f32 x);
    static i32 getSeed();
};
}