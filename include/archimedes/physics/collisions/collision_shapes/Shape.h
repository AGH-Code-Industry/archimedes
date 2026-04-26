#pragma once
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

///@brief Shape that can collide with other shapes
struct Shape {
	virtual ~Shape() = default;

	inline float2 getProjection(float3 axis, const TransformComponent& transform) const {
		return float2{0.0f, 0.0f};
	}
};
} // namespace arch::physics
