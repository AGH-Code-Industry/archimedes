#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include <archimedes/math/MathException.h>
#include <archimedes/math/PerlinNoise2D.h>

namespace arch::math {

i32 PerlinNoise2D::_generateSeed() {
    return std::chrono::system_clock::now().time_since_epoch().count();
}

void PerlinNoise2D::build(u32 gridSize, f32 minOffset, f32 maxOffset){
	build(gridSize, minOffset, maxOffset, _generateSeed());
}

void PerlinNoise2D::build(u32 gridSize, f32 minOffset, f32 maxOffset, i32 seed) {
    if (gridSize == 0) {
        throw MathException("Perlin Noise: gridSize must be greater than 0");
    }
	if (minOffset > maxOffset) {
		throw MathException("Perlin Noise: minOffset must be less than or equal to maxOffset");
	}
    _gridSize = gridSize;
    _seed = seed;
    _rng.seed(seed);
	_distribution = std::uniform_real_distribution<f32>(minOffset, maxOffset);
	_generatePermutation();
	_generateOffsets();
}

void PerlinNoise2D::_generatePermutation() {
    _permutation.clear();
	_permutation.reserve(_gridSize);
	for (i32 i = 0; i < _gridSize; i++) {
		_permutation.push_back(i);
	}
	std::ranges::shuffle(_permutation, _rng);
}

void PerlinNoise2D::_generateOffsets() {
    _offsets.clear();
	_offsets.resize(_gridSize);
	for (i32 i = 0; i < _gridSize; i++) {
		_offsets[i].reserve(_gridSize);
		for (i32 j = 0; j < _gridSize; j++) {
			_offsets[i].push_back(_distribution(_rng));
		}
	}
}

u32 PerlinNoise2D::_getHash(u32 x, u32 y) {
	u32 index = (_permutation[x] + y) % _gridSize;
	return _permutation[index];
}

GridCell PerlinNoise2D::_getGridCell(f32 x, f32 y) {
    f32 xFloor = floor(x);
    f32 yFloor = floor(y);

    f32 gridSize = _permutation.size();

    u32 gridX = glm::mod(xFloor, gridSize);
    u32 gridY = glm::mod(yFloor, gridSize);

    return {
        gridX,
        gridY,
        x - xFloor,
        y - yFloor
    };
}

f32 PerlinNoise2D::_getCornerContribution(
    const GridCell& cell,
    u32 offsetX,
    u32 offsetY
) {
    u32 cornerX = (cell.gridX + offsetX) % _gridSize;
    u32 cornerY = (cell.gridY + offsetY) % _gridSize;
    u32 hash = _getHash(cornerX, cornerY);

    float2 distance = {
        cell.localX - (f32)offsetX,
        cell.localY - (f32)offsetY
    };

    f32 gradientDot = glm::dot(
        distance,
        _constantVectors[hash % 4]
    );

    return gradientDot + _offsets[cornerX][cornerY];
}

f32 PerlinNoise2D::_interpolateCell(
    f32 bottomLeft,
    f32 bottomRight,
    f32 topLeft,
    f32 topRight,
    f32 localX,
    f32 localY
) {
    f32 percentageX = fade(localX);
    f32 percentageY = fade(localY);

    f32 left = glm::mix(bottomLeft, topLeft, percentageY);
    f32 right = glm::mix(bottomRight, topRight, percentageY);

    return glm::mix(left, right, percentageX);
}

f32 PerlinNoise2D::_generateOctave(f32 x, f32 y) {
    GridCell cell = _getGridCell(x, y);

    f32 bottomLeft  = _getCornerContribution(cell, 0, 0);
    f32 bottomRight = _getCornerContribution(cell, 1, 0);
    f32 topLeft     = _getCornerContribution(cell, 0, 1);
    f32 topRight    = _getCornerContribution(cell, 1, 1);

    return _interpolateCell(
        bottomLeft,
        bottomRight,
        topLeft,
        topRight,
        cell.localX,
        cell.localY
    );
}

f32 PerlinNoise2D::generate(f32 x, f32 y) {
	f32 result = 0.0f;
	f32 currentFrequency = baseFrequency;
	f32 currentAmplitude = baseAmplitude;
	for (u32 octave = 0; octave < octaves; octave++) {
		result += currentAmplitude * _generateOctave(x * currentFrequency, y * currentFrequency);
		currentAmplitude *= amplitudeFactor;
		currentFrequency *= frequencyFactor;
	}
	return std::clamp(result, minResult, maxResult);
}

i32 PerlinNoise2D::getSeed() {
    return _seed;
}
} // namespace arch::math
