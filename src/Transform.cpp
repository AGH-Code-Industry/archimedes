#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace arch {

std::pair<Quaternion, Quaternion> Transform::rotate(f32 angle, f32 x, f32 y, f32 z) {
	// normalize x,y,z
	auto norm = static_cast<f32>(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
	x /= norm;
	y /= norm;
	z /= norm;

	// fix angle
	angle /= 2;
	const f64 pi = std::acos(-1);
	angle *= static_cast<f32>(pi / 180.0);

	// create rotation quaternions
	f32 a = x * std::sin(angle);
	f32 b = y * std::sin(angle);
	f32 c = z * std::sin(angle);
	float3 imaginaryPart = float3(a, b, c);
	float4 q = float4(imaginaryPart, std::cos(angle));
	float4 q1 = float4(-imaginaryPart, std::cos(angle));

	return { q, q1 };
}

std::pair<Quaternion, Quaternion> Transform::rotate(f32 angle, float3 rotationAxis) {
	return rotate(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z);
}

Mat4x4 Transform::translate(float3 translationVector) {
	Mat4x4 translationMatrix = glm::translate(Mat4x4(1.0f), translationVector);
	return translationMatrix;
}

Mat4x4 Transform::translate(f32 x, f32 y, f32 z) {
	float3 translationVector = float3(x, y, z);

	return translate(translationVector);
}

Mat4x4 Transform::scale(float3 scalingVector) {
	Mat4x4 scalingMatrix = glm::scale(Mat4x4(1.0f), scalingVector);

	return scalingMatrix;
}

Mat4x4 Transform::scale(f32 x, f32 y, f32 z) {
	float3 scalingVector = float3(x, y, z);

	return scale(scalingVector);
}

Mat4x4 Transform::scale(f32 a) {
	auto scalingVector = float3(a);

	return scale(scalingVector);
}

} // namespace arch
