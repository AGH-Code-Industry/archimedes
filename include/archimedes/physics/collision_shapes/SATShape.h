#pragma once
#include <archimedes/Mmath.h>
#include <archimedes/physics/collision_shapes/Shape.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {
using TransformComponent = arch::scene::components::TransformComponent;

struct SATShape: public Shape {
	virtual float2 getProjection(float3 axis, const TransformComponent& transform) const = 0;
};
} // namespace arch::physics
