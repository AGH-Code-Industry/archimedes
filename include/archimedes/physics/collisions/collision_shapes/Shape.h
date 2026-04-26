#pragma once
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = scene::components::TransformComponent;

///@brief Shape that can collide with other shapes
struct Shape {
	virtual ~Shape() = default;

	virtual float2 getProjection(float3 axis, const TransformComponent& transform) const = 0;
};
} // namespace arch::physics
