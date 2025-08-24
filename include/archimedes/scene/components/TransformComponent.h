#pragma once
#include <archimedes/ArchMath.h>

namespace arch::scene::components {

struct TransformComponent {
	float3 position;
	Quat rotation;
	float3 scale;

	Mat4x4 getTransformMatrix() const noexcept {
		return glm::translate(Mat4x4(1.f), position) * glm::toMat4(rotation) * glm::scale(Mat4x4(1.f), scale);
	}
};

} // namespace arch::scene::components
