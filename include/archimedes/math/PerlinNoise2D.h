#pragma once
#include <archimedes/math/Math.h>
#include <random>
#include <vector>


namespace arch::math {

struct GridCell {
    u32 gridX;
    u32 gridY;
    f32 localX;
    f32 localY;
};

class PerlinNoise2D
{
    inline static std::mt19937 _rng;
    inline static std::uniform_real_distribution<f32> _distribution;
    inline static std::vector<u32> _permutation;
    inline static std::vector<std::vector<f32>> _offsets;
    inline static u32 _gridSize;

    inline static const std::array<float2, 4> _constantVectors = {
        float2(1.0, 1.0),
        float2(-1.0, 1.0),
        float2(-1.0, -1.0),
        float2(1.0, -1.0),
    };

    static i32 _getSeed();
    static u32 _getHash(u32 x, u32 y);
    static void _createPermutation();
    static void _createOffsets();
    static f32 _generateOctave(f32 x, f32 y);

    static GridCell _getGridCell(f32 x, f32 y);
    static f32 _cornerContribution(const GridCell& cell, u32 offsetX, u32 offsetY);
    static f32 _interpolateCell(f32 bottomLeft, f32 bottomRight, f32 topLeft, f32 topRight, f32 localX, f32 localY);

public:
    inline static u32 octaves = 6;
    inline static f32 baseAmplitude = 0.6f;
    inline static f32 baseFrequency = 0.005f;
    inline static f32 amplitudeFactor = 0.5f;
    inline static f32 frequencyFactor = 2.0f;
    inline static f32 minResult = -1.0f;
    inline static f32 maxResult = 1.0f;

    static void build(u32 gridSize, f32 minOffset, f32 maxOffset);
    static void build(u32 gridSize, f32 minOffset, f32 maxOffset, i32 seed);
    static f32 generate(f32 x, f32 y);
};
}