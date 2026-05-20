#pragma once
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

///@brief Shape that can collide with other shapes
struct Shape {
	virtual ~Shape() = default;

	inline float2 getProjection(const TransformComponent& transform, float2 axis) const {
		return float2{0.0f, 0.0f};
	}

	inline bool containsPoint(const TransformComponent& transform, float2 point) const { 
		return false;
	}
};
} // namespace arch::physics
