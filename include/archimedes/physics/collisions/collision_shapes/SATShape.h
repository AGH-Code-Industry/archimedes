#pragma once
#include <archimedes/Mmath.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {
using TransformComponent = arch::scene::components::TransformComponent;

///@brief Virtual class for shaped tested by SAT collision algorithm
struct SATShape: public Shape {
	///@brief Project the shape on the axis
	virtual float2 getProjection(float3 axis, const TransformComponent& transform) const = 0;
};
} // namespace arch::physics
