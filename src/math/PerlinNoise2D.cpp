#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include <archimedes/math/MathException.h>
#include <archimedes/math/PerlinNoise2D.h>

namespace arch::math {

void PerlinNoise2D::_createPermutation(i32 size) {
	_permutation.reserve(size);
	for (i32 i = 0; i < size; i++) {
		_permutation.push_back(i);
	}
	std::ranges::shuffle(_permutation, _rng);
}

void PerlinNoise2D::_createOffsets(i32 size) {
	_offsets.resize(size);
	for (i32 i = 0; i < size; i++) {
		_offsets[i].reserve(size);
		for (i32 j = 0; j < size; j++) {
			_offsets[i].push_back(_distribution(_rng));
		}
	}
}

f32 PerlinNoise2D::_getOffset(i32 x, i32 y) {
	i32 size = _offsets.size();
	x = (x + size) % size;
	y = (y + size) % size;
	return _offsets[x][y];
}

i32 PerlinNoise2D::_getHash(i32 x, i32 y) {
	i32 size = _permutation.size();
	x = (x + size) % size;
	y = (y + size) % size;
	i32 index = (_permutation[x] + y) % size;
	return _permutation[index];
}

f32 PerlinNoise2D::_fade(f32 t)
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

i32 PerlinNoise2D::_getSeed() {
    return (i32)std::chrono::system_clock::now().time_since_epoch().count();
}

void PerlinNoise2D::build(i32 permutationSize, f32 minOffset, f32 maxOffset){
	build(permutationSize, minOffset, maxOffset, _getSeed());
}

void PerlinNoise2D::build(i32 permutationSize, f32 minOffset, f32 maxOffset, i32 seed) {
	_rng.seed(seed);

	if (minOffset > maxOffset) {
		throw MathException("Perlin Noise: minOffset must be less than or equal to maxOffset");
	}

	_distribution = std::uniform_real_distribution<f32>(minOffset, maxOffset);

	_createPermutation(permutationSize);
	_createOffsets(permutationSize);
}

GridCell PerlinNoise2D::_getGridCell(f32 x, f32 y) {
    const f32 xFloor = floor(x);
    const f32 yFloor = floor(y);

    const i32 permutationSize = (i32)_permutation.size();

    const i32 gridX = ((i32)xFloor + permutationSize) % permutationSize;
    const i32 gridY = ((i32)yFloor + permutationSize) % permutationSize;

    return {
        gridX,
        gridY,
        x - xFloor,
        y - yFloor
    };
}

f32 PerlinNoise2D::_cornerContribution(
    const GridCell& cell,
    i32 offsetX,
    i32 offsetY
) {
    const i32 hash = _getHash(cell.gridX + offsetX, cell.gridY + offsetY);

    const float2 distance = {
        cell.localX - (f32)offsetX,
        cell.localY - (f32)offsetY
    };

    const f32 gradientDot = glm::dot(
        distance,
        _constantVectors[hash % 4]
    );

    return gradientDot + _getOffset(cell.gridX + offsetX, cell.gridY + offsetY);
}

f32 PerlinNoise2D::_interpolateCell(
    f32 bottomLeft,
    f32 bottomRight,
    f32 topLeft,
    f32 topRight,
    f32 localX,
    f32 localY
) {
    const f32 percentageX = _fade(localX);
    const f32 percentageY = _fade(localY);

    const f32 left = glm::mix(bottomLeft, topLeft, percentageY);
    const f32 right = glm::mix(bottomRight, topRight, percentageY);

    return glm::mix(left, right, percentageX);
}

f32 PerlinNoise2D::_generateOctave(f32 x, f32 y) {
    const GridCell cell = _getGridCell(x, y);

    const f32 bottomLeft  = _cornerContribution(cell, 0, 0);
    const f32 bottomRight = _cornerContribution(cell, 1, 0);
    const f32 topLeft     = _cornerContribution(cell, 0, 1);
    const f32 topRight    = _cornerContribution(cell, 1, 1);

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
	for (i32 octave = 0; octave < octaves; octave++) {
		result += currentAmplitude * _generateOctave(x * currentFrequency, y * currentFrequency);
		currentAmplitude *= amplitudeFactor;
		currentFrequency *= frequencyFactor;
	}
	result = std::clamp(result, minResult, maxResult);
	return result;
}
} // namespace arch::math
