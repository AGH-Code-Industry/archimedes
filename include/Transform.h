#pragma once

#include <utility>

#include "ArchMath.h"

namespace arch {

using Quaternion = float4;

class Transform {
public:
	/**
	 * Calculates quaternions needed for rotation relative to given vector components
	 * @param angle rotation angle in degrees
	 * @param x x component of the vector generating rotation axis
	 * @param y y component of the vector generating rotation axis
	 * @param z z component of the vector generating rotation axis
	 * @return Quaternions structure with (q, q^-1)
	 */
	static std::pair<Quaternion, Quaternion> rotate(f32 angle, f32 x, f32 y, f32 z);

	/**
	 * Calculates quaternions needed for rotation relative to given vector components
	 * @param angle rotation angle in degrees
	 * @param rotationAxis vector generating rotation axis
	 * @return Quaternions structure with (q, q^-1)
	 */
	static std::pair<Quaternion, Quaternion> rotate(f32 angle, float3 rotationAxis);

	/**
	 * Creates matrix needed to shift point by given vector.
	 * @param translationVector vector used to translate point
	 * @return arch::math::Mat4x4 needed for translating point by given vector
	 */
	static Mat4x4 translate(float3 translationVector);

	/**
	 * Creates matrix needed to shift point by given vector.
	 * @param x x component of translation vector
	 * @param y y component of translation vector
	 * @param z z component of translation vector
	 * @return arch::math::Mat4x4 needed for translating point by given vector
	 */
	static Mat4x4 translate(f32 x, f32 y, f32 z);
	/**
	 * Creates matrix ideal for scaling objects by given values
	 * @param scalingVector Vector with scaling factors.
	 * @return arch::math::Mat4x4 matrix needed for scaling object by given values for each direction
	 */
	static Mat4x4 scale(float3 scalingVector);

	/**
	 * Creates matrix ideal for scaling objects by given values
	 * @param x x factor
	 * @param y y factor
	 * @param z z factor
	 * @return arch::math::Mat4x4 matrix needed for scaling object by given values for each direction
	 */
	static Mat4x4 scale(f32 x, f32 y, f32 z);

	/**
	 * Creates matrix ideal for scaling objects in x,y,z directions by given value
	 * @param a scaling factor
	 * @return arch::math::Mat4x4 matrix needed for scaling object by given value for each direction
	 */
	static Mat4x4 scale(f32 a);
};

} // namespace arch
