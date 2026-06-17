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

f32 PerlinNoise2D::_getOffset(i32 X, i32 Y) const {
	X %= _offsets.size();
	Y %= _offsets.size();
	return _offsets[X][Y];
}

i32 PerlinNoise2D::_getHash(i32 x, i32 y) const {
	x %= _permutation.size();
	y %= _permutation.size();
	i32 index = (_permutation[x] + y) % _permutation.size();
	return _permutation[index];
}

f32 PerlinNoise2D::_fade(f32 t)
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

i32 PerlinNoise2D::_getSeed() const {
    return (i32)std::chrono::system_clock::now().time_since_epoch().count();
}

PerlinNoise2D::PerlinNoise2D(i32 permutationSize, f32 minOffset, f32 maxOffset):
	PerlinNoise2D(permutationSize, minOffset, maxOffset, _getSeed()) {}

PerlinNoise2D::PerlinNoise2D(i32 permutationSize, f32 minOffset, f32 maxOffset, i32 seed) {
	_rng.seed(seed);

	if (minOffset > maxOffset) {
		throw MathException("Perlin Noise: minOffset must be less than or equal to maxOffset");
	}

	_distribution = std::uniform_real_distribution<f32>(minOffset, maxOffset);

	_createPermutation(permutationSize);
	_createOffsets(permutationSize);
}

f32 PerlinNoise2D::_generateOctave(f32 x, f32 y) const {
	f32 xRounded = floor(x);
	f32 yRounded = floor(y);

	i32 gridX = (i32)(xRounded) % _permutation.size();
	i32 gridY = (i32)(yRounded) % _permutation.size();

	f32 cellX = x - xRounded;
	f32 cellY = y - yRounded;

	float2 topRight = {cellX - 1.0f, cellY - 1.0f};
	float2 topLeft = {cellX, cellY - 1.0f};
	float2 bottomRight = {cellX - 1.0f, cellY};
	float2 bottomLeft = {cellX, cellY};

	i32 hashTopRight = _getHash(gridX + 1, gridY + 1);
	i32 hashTopLeft = _getHash(gridX, gridY + 1);
	i32 hashBottomRight = _getHash(gridX + 1, gridY);
	i32 hashBottomLeft = _getHash(gridX, gridY);

	f32 dotTopRight = glm::dot(topRight, _constantVectors[hashTopRight % 4]);
	f32 dotTopLeft = glm::dot(topLeft, _constantVectors[hashTopLeft % 4]);
	f32 dotBottomRight = glm::dot(bottomRight, _constantVectors[hashBottomRight % 4]);
	f32 dotBottomLeft = glm::dot(bottomLeft, _constantVectors[hashBottomLeft % 4]);

	dotTopRight += _getOffset(gridX + 1, gridY + 1);
	dotTopLeft += _getOffset(gridX, gridY + 1);
	dotBottomRight += _getOffset(gridX + 1, gridY);
	dotBottomLeft += _getOffset(gridX, gridY);

	f32 percentageX = _fade(cellX);
	f32 percentageY = _fade(cellY);

    return glm::mix(
        glm::mix(dotBottomLeft, dotTopLeft, percentageY),
        glm::mix(dotBottomRight, dotTopRight, percentageY),
        percentageX
    );
}

f32 PerlinNoise2D::generate(f32 x, f32 y) {
	f32 result = 0.0f;
	for (i32 octave = 0; octave < octaves; octave++) {
		result += amplitude * _generateOctave(x * frequency, y * frequency);
		amplitude *= amplitudeFactor;
		frequency *= frequencyFactor;
	}
	result = std::clamp(result, minResult, maxResult);
	return result;
}
} // namespace arch::math
