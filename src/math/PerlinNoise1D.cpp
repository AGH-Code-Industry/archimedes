#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

#include <archimedes/math/MathException.h>
#include <archimedes/math/PerlinNoise1D.h>

namespace arch::math {

i32 PerlinNoise1D::_generateSeed() {
    return std::chrono::system_clock::now().time_since_epoch().count();
}

void PerlinNoise1D::build(u32 nodesSize, f32 minOffset, f32 maxOffset){
	build(nodesSize, minOffset, maxOffset, _generateSeed());
}

void PerlinNoise1D::build(u32 nodesSize, f32 minOffset, f32 maxOffset, i32 seed) {
    build(nodesSize, seed);
    _generateOffsets(minOffset, maxOffset);
}

void PerlinNoise1D::build(u32 nodesSize){
    build(nodesSize, _generateSeed());
}

void PerlinNoise1D::build(u32 nodesSize, i32 seed){
    if (nodesSize == 0) {
        throw MathException("Perlin Noise 1D: nodesSize must be greater than 0");
    }
    _nodesSize = nodesSize;
    _seed = seed;
    _rng.seed(seed);
    _generateGradients();
}

void PerlinNoise1D::_generateGradients(){
    _gradients.clear();
    _gradients.reserve(_nodesSize);
    for (i32 i=0; i<_nodesSize; i++){
        _gradients.push_back(_gradientsDistribution(_rng));
    }
}

void PerlinNoise1D::_generateOffsets(f32 minOffset, f32 maxOffset) {
    if (minOffset > maxOffset) {
		throw MathException("Perlin Noise 1D: minOffset must be less than or equal to maxOffset");
	}
	auto distribution = std::uniform_real_distribution<f32>(minOffset, maxOffset);
    _offsets.clear();
    _offsets.reserve(_nodesSize);
    for (i32 i=0; i<_nodesSize; i++){
        _offsets.push_back(distribution(_rng));
    }
}

f32 PerlinNoise1D::_generateOctave(f32 x) {
    f32 xFloor = floor(x);
    u32 leftNode = glm::mod(xFloor, (f32)_nodesSize);
    f32 distance = x - xFloor;
    u32 rightNode = (leftNode + 1) % _nodesSize;
    f32 leftOffset = _offsets.empty() ? 0.0f : _offsets[leftNode];
    f32 rightOffset = _offsets.empty() ? 0.0f : _offsets[rightNode];
    f32 leftContribution =
        leftOffset + _gradients[leftNode] * distance;
    f32 rightContribution =
        rightOffset + _gradients[rightNode] * (distance - 1.0f);
    f32 percentage = fade(distance);
    return glm::mix(leftContribution, rightContribution, percentage);
}

f32 PerlinNoise1D::generate(f32 x) {
    if(_gradients.empty()) {
        throw MathException("Perlin Noise 1D: build() must be called before generate()");
    }
	f32 result = 0.0f;
	f32 currentFrequency = baseFrequency;
	f32 currentAmplitude = baseAmplitude;
	for (u32 octave = 0; octave < octaves; octave++) {
		result += currentAmplitude * _generateOctave(x * currentFrequency);
		currentAmplitude *= amplitudeFactor;
		currentFrequency *= frequencyFactor;
	}
	return std::clamp(result, minResult, maxResult);
}

i32 PerlinNoise1D::getSeed() {
    return _seed;
}
} // namespace arch::math
