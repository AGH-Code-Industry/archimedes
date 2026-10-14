#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include <archimedes/algorithms/AlgorithmException.h>
#include <archimedes/algorithms/PerlinNoise2D.h>

namespace arch::algorithms {

i32 PerlinNoise2D::_generateSeed() {
	std::random_device device;
	std::uniform_int_distribution<i32> distribution;
	return distribution(device);
}

void PerlinNoise2D::build(u32 gridSize, f32 minOffset, f32 maxOffset) {
	build(gridSize, minOffset, maxOffset, _generateSeed());
}

void PerlinNoise2D::build(u32 gridSize, f32 minOffset, f32 maxOffset, i32 seed) {
	if (minOffset > maxOffset) {
		throw AlgorithmException("Perlin Noise 2D: minOffset must be less than or equal to maxOffset");
	}
	build(gridSize, seed);
	_generateOffsets(minOffset, maxOffset);
}

void PerlinNoise2D::build(u32 gridSize) {
	build(gridSize, _generateSeed());
}

void PerlinNoise2D::build(u32 gridSize, i32 seed) {
	if (gridSize == 0) {
		throw AlgorithmException("Perlin Noise 2D: gridSize must be greater than 0");
	}
	_gridSize = gridSize;
	_seed = seed;
	_rng.seed(seed);
	_permutation.clear();
	_offsets.clear();
	_generatePermutation();
}

void PerlinNoise2D::_generatePermutation() {
	_permutation.reserve(_gridSize);
	for (i32 i = 0; i < _gridSize; i++) {
		_permutation.push_back(i);
	}
	std::ranges::shuffle(_permutation, _rng);
}

void PerlinNoise2D::_generateOffsets(f32 minOffset, f32 maxOffset) {
	auto distribution = std::uniform_real_distribution<f32>(minOffset, maxOffset);
	_offsets.resize(_gridSize);
	for (i32 i = 0; i < _gridSize; i++) {
		_offsets[i].reserve(_gridSize);
		for (i32 j = 0; j < _gridSize; j++) {
			_offsets[i].push_back(distribution(_rng));
		}
	}
}

u32 PerlinNoise2D::_getHash(u32 x, u32 y) const {
	u32 index = (_permutation[x] + y) % _gridSize;
	return _permutation[index];
}

GridCell PerlinNoise2D::_getGridCell(f32 x, f32 y) const {
	f32 xFloor = floor(x);
	f32 yFloor = floor(y);

	f32 size = _gridSize;

	u32 gridX = glm::mod(xFloor, size);
	u32 gridY = glm::mod(yFloor, size);

	return { gridX, gridY, x - xFloor, y - yFloor };
}

f32 PerlinNoise2D::_getCornerContribution(const GridCell& cell, u32 offsetX, u32 offsetY) const {
	u32 cornerX = (cell.gridX + offsetX) % _gridSize;
	u32 cornerY = (cell.gridY + offsetY) % _gridSize;
	u32 hash = _getHash(cornerX, cornerY);

	float2 distance = { cell.localX - (f32)offsetX, cell.localY - (f32)offsetY };

	f32 gradientDot = glm::dot(distance, _constantVectors[hash % 4]);

	f32 offset = _offsets.empty() ? 0.0f : _offsets[cornerX][cornerY];
	return gradientDot + offset;
}

f32 PerlinNoise2D::_interpolateCell(
	f32 bottomLeft,
	f32 bottomRight,
	f32 topLeft,
	f32 topRight,
	f32 localX,
	f32 localY
) const {
	f32 percentageX = fade(localX);
	f32 percentageY = fade(localY);

	f32 left = glm::mix(bottomLeft, topLeft, percentageY);
	f32 right = glm::mix(bottomRight, topRight, percentageY);

	return glm::mix(left, right, percentageX);
}

f32 PerlinNoise2D::_generateOctave(f32 x, f32 y) const {
	GridCell cell = _getGridCell(x, y);

	f32 bottomLeft = _getCornerContribution(cell, 0, 0);
	f32 bottomRight = _getCornerContribution(cell, 1, 0);
	f32 topLeft = _getCornerContribution(cell, 0, 1);
	f32 topRight = _getCornerContribution(cell, 1, 1);

	return _interpolateCell(bottomLeft, bottomRight, topLeft, topRight, cell.localX, cell.localY);
}

f32 PerlinNoise2D::generate(f32 x, f32 y) const {
	if (_permutation.empty()) {
		throw AlgorithmException("Perlin Noise 2D: build() must be called before generate()");
	}
	f32 result = 0.0f;
	f32 currentFrequency = _config.baseFrequency;
	f32 currentAmplitude = _config.baseAmplitude;
	for (u32 octave = 0; octave < _config.octaves; octave++) {
		result += currentAmplitude * _generateOctave(x * currentFrequency, y * currentFrequency);
		currentAmplitude *= _config.amplitudeFactor;
		currentFrequency *= _config.frequencyFactor;
	}
	return std::clamp(result, _config.minResult, _config.maxResult);
}

i32 PerlinNoise2D::getSeed() const {
	return _seed;
}

f32 PerlinNoise2D::getMinResult() const {
	return _config.minResult;
}

f32 PerlinNoise2D::getMaxResult() const {
	return _config.maxResult;
}

PerlinNoise2D& PerlinNoise2D::setOctaves(u32 octaves) {
	_config.octaves = octaves;
	return *this;
}

PerlinNoise2D& PerlinNoise2D::setBaseAmplitude(f32 baseAmplitude) {
	_config.baseAmplitude = baseAmplitude;
	return *this;
}

PerlinNoise2D& PerlinNoise2D::setBaseFrequency(f32 baseFrequency) {
	_config.baseFrequency = baseFrequency;
	return *this;
}

PerlinNoise2D& PerlinNoise2D::setAmplitudeFactor(f32 amplitudeFactor) {
	_config.amplitudeFactor = amplitudeFactor;
	return *this;
}

PerlinNoise2D& PerlinNoise2D::setFrequencyFactor(f32 frequencyFactor) {
	_config.frequencyFactor = frequencyFactor;
	return *this;
}

PerlinNoise2D& PerlinNoise2D::setMinResult(f32 minResult) {
	_config.minResult = minResult;
	return *this;
}

PerlinNoise2D& PerlinNoise2D::setMaxResult(f32 maxResult) {
	_config.maxResult = maxResult;
	return *this;
}
} // namespace arch::algorithms
