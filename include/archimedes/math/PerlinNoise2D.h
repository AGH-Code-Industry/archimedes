#pragma once
#include <archimedes/math/Math.h>
#include <random>
#include <vector>


namespace arch::math {
class PerlinNoise2D
{
    std::mt19937 _rng;
    std::uniform_real_distribution<f32> _distribution;
    std::vector<i32> _permutation;
    std::vector<std::vector<f32>> _offsets;

    const std::array<float2, 4> _constantVectors = {
        float2(1.0, 1.0),
        float2(-1.0, 1.0),
        float2(-1.0, -1.0),
        float2(1.0, -1.0),
    };

    i32 _getSeed() const;
    i32 _getHash(i32 x, i32 y) const;
    void _createPermutation(i32 size);
    void _createOffsets(i32 size);
    f32 _getOffset(i32 x, i32 y) const;
    static f32 _fade(f32 t);
    f32 _generateOctave(f32 x, f32 y) const;

public:
    i32 octaves = 6;
    f32 amplitude = 0.6f;
    f32 frequency = 0.005f;
    f32 amplitudeFactor = 0.5f;
    f32 frequencyFactor = 2.0f;
    f32 minResult = -1.0f;
    f32 maxResult = 1.0f;

    PerlinNoise2D(i32 permutationSize, f32 minOffset, f32 maxOffset);
    PerlinNoise2D(i32 permutationSize, f32 minOffset, f32 maxOffset, i32 seed);
    f32 generate(f32 x, f32 y);
};
}