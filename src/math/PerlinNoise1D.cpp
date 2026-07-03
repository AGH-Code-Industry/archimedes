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
    if (nodesSize == 0) {
        throw MathException("Perlin Noise 1D: nodesSize must be greater than 0");
    }
	if (minOffset > maxOffset) {
		throw MathException("Perlin Noise 1D: minOffset must be less than or equal to maxOffset");
	}
    _nodesSize = nodesSize;
    _seed = seed;
    _rng.seed(seed);
	_offsetsDistribution = std::uniform_real_distribution<f32>(minOffset, maxOffset);
    _generateGradientsAndOffsets();
}

void PerlinNoise1D::_generateGradientsAndOffsets(){
    _gradients.clear();
    _gradients.reserve(_nodesSize);
    _offsets.clear();
    _offsets.reserve(_nodesSize);
    for (i32 i=0; i<_nodesSize; i++){
        _gradients.push_back(_gradientsDistribution(_rng));
        _offsets.push_back(_offsetsDistribution(_rng));
    }
}

f32 PerlinNoise1D::_generateOctave(f32 x) {
    f32 xFloor = floor(x);
    u32 leftNode = glm::mod(xFloor, (f32)_nodesSize);
    f32 distance = x - xFloor;
    u32 rightNode = (leftNode + 1) % _nodesSize;
    f32 leftContribution =
        _offsets[leftNode] + _gradients[leftNode] * distance;
    f32 rightContribution =
        _offsets[rightNode] + _gradients[rightNode] * (distance - 1.0f);
    f32 percentage = fade(distance);
    return glm::mix(leftContribution, rightContribution, percentage);
}

f32 PerlinNoise1D::generate(f32 x) {
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
