#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cmath>

namespace arch::math {

using Byte = std::byte;

// NOLINTBEGIN(*-identifier-naming)

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using f32 = float_t;
using f64 = double_t;
using fld = long double;

using float2 = glm::vec2;
using float3 = glm::vec3;
using float4 = glm::vec4;

using double2 = glm::dvec2;
using double3 = glm::dvec3;
using double4 = glm::dvec4;

using int2 = glm::ivec2;
using int3 = glm::ivec3;
using int4 = glm::ivec4;

using uint2 = glm::uvec2;
using uint3 = glm::uvec3;
using uint4 = glm::uvec4;

// NOLINTEND(*-identifier-naming)

using Mat2x2 = glm::mat2;
using Mat3x3 = glm::mat3;
using Mat4x4 = glm::mat4;

using Color = glm::vec4;

using Quat = glm::qua<f32>;

inline f32 fade(f32 t){
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

/// @brief Returns a quaternion from a rotation angle.
/// @param angle The angle in radians.
inline Quat quaternion(f32 angle) {
	return glm::angleAxis(angle, float3(0.0f, 0.0f, 1.0f));
}

inline float4 hsvToRgb(f32 hue, f32 saturation, f32 value) {
	f32 chroma = value * saturation;
	f32 secondaryColorComponent = chroma * (1.f - std::fabs(std::fmod(hue / 60.f, 2.f) - 1.f));
	f32 brightnessOffset = value - chroma;

    float3 rgb;

	if (hue < 60) {
        rgb = {chroma, secondaryColorComponent, 0.0f};
	} else if (hue < 120) {
        rgb = {secondaryColorComponent, chroma, 0.0f};
	} else if (hue < 180) {
        rgb = {0.0f, chroma, secondaryColorComponent};
	} else if (hue < 240) {
        rgb = {0.0f, secondaryColorComponent, chroma};
	} else if (hue < 300) {
        rgb = {secondaryColorComponent, 0.0f, chroma};
	} else {
        rgb = {chroma, 0.0f, secondaryColorComponent};
	}

    return float4(rgb + brightnessOffset, 1.0f);
}

} // namespace arch::math
