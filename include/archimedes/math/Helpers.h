
#pragma once

#include <cmath>
#include <glm/gtc/quaternion.hpp>
#include <archimedes/math/Math.h>


namespace arch::math {
/// @brief Polynomial fade function used for smoothly blending between two values
/// You can use it as a factor for linear interpolation, like glm::mix function
/// @param t Interpolation parameter in the range [0, 1]
inline f32 fade(f32 t) {
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

/// @brief Returns a quaternion from a rotation angle.
/// @param angle The angle in radians.
inline Quat quaternion(f32 angle) {
	return glm::angleAxis(angle, float3(0.0f, 0.0f, 1.0f));
}

/// @brief Converts colors from HSV scale to RGB
/// @param hue Color's hue
/// @param saturation Color's saturation
/// @param value  Color's value
inline float4 hsvToRgb(f32 hue, f32 saturation, f32 value) {
	f32 chroma = value * saturation;
	f32 secondaryColorComponent = chroma * (1.f - std::fabs(std::fmod(hue / 60.f, 2.f) - 1.f));
	f32 brightnessOffset = value - chroma;

	float3 rgb;

	if (hue < 60) {
		rgb = { chroma, secondaryColorComponent, 0.0f };
	} else if (hue < 120) {
		rgb = { secondaryColorComponent, chroma, 0.0f };
	} else if (hue < 180) {
		rgb = { 0.0f, chroma, secondaryColorComponent };
	} else if (hue < 240) {
		rgb = { 0.0f, secondaryColorComponent, chroma };
	} else if (hue < 300) {
		rgb = { secondaryColorComponent, 0.0f, chroma };
	} else {
		rgb = { chroma, 0.0f, secondaryColorComponent };
	}

	return float4(rgb + brightnessOffset, 1.0f);
}
}