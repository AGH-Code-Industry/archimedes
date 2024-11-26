#pragma once
#include "ArchMath.h"

namespace arch::scene::components {

struct TransformComponent {
	float3 position;
	Quat rotation;
	float3 scale;

	float3& getPosition() noexcept { return position; }

	float3 getPosition() const noexcept { return position; }

	void setPosition(const float3& position) noexcept { this->position = position; }

	Quat& getRotation() noexcept { return rotation; }

	Quat getRotation() const noexcept { return rotation; }

	void setRotation(const Quat& rotation) noexcept { this->rotation = rotation; }

	float3& getScale() noexcept { return scale; }

	float3 getScale() const noexcept { return scale; }

	void setScale(const float3& scale) noexcept { this->scale = scale; }

	Mat4x4 getTransformMatrix() const noexcept {
		return glm::translate(Mat4x4(1.f), position) * glm::toMat4(rotation) * glm::scale(Mat4x4(1.f), scale);
	}
};

} // namespace arch::scene::components
