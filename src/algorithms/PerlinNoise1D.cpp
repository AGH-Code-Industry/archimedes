#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include <archimedes/algorithms/AlgorithmException.h>
#include <archimedes/algorithms/PerlinNoise1D.h>

namespace arch::algorithms {

i32 PerlinNoise1D::_generateSeed() {
	std::random_device device;
	std::uniform_int_distribution<i32> distribution;
	return distribution(device);
}

void PerlinNoise1D::build(u32 nodesSize, f32 minOffset, f32 maxOffset) {
	build(nodesSize, minOffset, maxOffset, _generateSeed());
}

void PerlinNoise1D::build(u32 nodesSize, f32 minOffset, f32 maxOffset, i32 seed) {
	if (minOffset > maxOffset) {
		throw AlgorithmException("Perlin Noise 1D: minOffset must be less than or equal to maxOffset");
	}
	build(nodesSize, seed);
	_generateOffsets(minOffset, maxOffset);
}

void PerlinNoise1D::build(u32 nodesSize) {
	build(nodesSize, _generateSeed());
}

void PerlinNoise1D::build(u32 nodesSize, i32 seed) {
	if (nodesSize == 0) {
		throw AlgorithmException("Perlin Noise 1D: nodesSize must be greater than 0");
	}
	_nodesSize = nodesSize;
	_seed = seed;
	_rng.seed(seed);
	_offsets.clear();
	_gradients.clear();
	_generateGradients();
}

void PerlinNoise1D::_generateGradients() {
	_gradients.reserve(_nodesSize);
	for (i32 i = 0; i < _nodesSize; i++) {
		_gradients.push_back(_gradientsDistribution(_rng));
	}
}

void PerlinNoise1D::_generateOffsets(f32 minOffset, f32 maxOffset) {
	auto distribution = std::uniform_real_distribution<f32>(minOffset, maxOffset);
	_offsets.reserve(_nodesSize);
	for (i32 i = 0; i < _nodesSize; i++) {
		_offsets.push_back(distribution(_rng));
	}
}

f32 PerlinNoise1D::_generateOctave(f32 x) const {
	f32 xFloor = floor(x);
	u32 leftNode = glm::mod(xFloor, (f32)_nodesSize);
	f32 distance = x - xFloor;
	u32 rightNode = (leftNode + 1) % _nodesSize;
	f32 leftOffset = _offsets.empty() ? 0.0f : _offsets[leftNode];
	f32 rightOffset = _offsets.empty() ? 0.0f : _offsets[rightNode];
	f32 leftContribution = leftOffset + _gradients[leftNode] * distance;
	f32 rightContribution = rightOffset + _gradients[rightNode] * (distance - 1.0f);
	f32 percentage = fade(distance);
	return glm::mix(leftContribution, rightContribution, percentage);
}

f32 PerlinNoise1D::generate(f32 x) const {
	if (_gradients.empty()) {
		throw AlgorithmException("Perlin Noise 1D: build() must be called before generate()");
	}
	f32 result = 0.0f;
	f32 currentFrequency = _config.baseFrequency;
	f32 currentAmplitude = _config.baseAmplitude;
	for (u32 octave = 0; octave < _config.octaves; octave++) {
		result += currentAmplitude * _generateOctave(x * currentFrequency);
		currentAmplitude *= _config.amplitudeFactor;
		currentFrequency *= _config.frequencyFactor;
	}
	return std::clamp(result, _config.minResult, _config.maxResult);
}

i32 PerlinNoise1D::getSeed() const {
	return _seed;
}

f32 PerlinNoise1D::getMinResult() const {
	return _config.minResult;
}

f32 PerlinNoise1D::getMaxResult() const {
	return _config.maxResult;
}

PerlinNoise1D& PerlinNoise1D::setOctaves(u32 octaves) {
	_config.octaves = octaves;
	return *this;
}

PerlinNoise1D& PerlinNoise1D::setBaseAmplitude(f32 baseAmplitude) {
	_config.baseAmplitude = baseAmplitude;
	return *this;
}

PerlinNoise1D& PerlinNoise1D::setBaseFrequency(f32 baseFrequency) {
	_config.baseFrequency = baseFrequency;
	return *this;
}

PerlinNoise1D& PerlinNoise1D::setAmplitudeFactor(f32 amplitudeFactor) {
	_config.amplitudeFactor = amplitudeFactor;
	return *this;
}

PerlinNoise1D& PerlinNoise1D::setFrequencyFactor(f32 frequencyFactor) {
	_config.frequencyFactor = frequencyFactor;
	return *this;
}

PerlinNoise1D& PerlinNoise1D::setMinResult(f32 minResult) {
	_config.minResult = minResult;
	return *this;
}

PerlinNoise1D& PerlinNoise1D::setMaxResult(f32 maxResult) {
	_config.maxResult = maxResult;
	return *this;
}

} // namespace arch::algorithms
