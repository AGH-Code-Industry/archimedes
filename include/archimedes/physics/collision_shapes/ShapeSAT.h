#pragma once
#include <archimedes/Mmath.h>
#include <archimedes/physics/collision_shapes/Shape.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {

using TransformComponent = arch::scene::components::TransformComponent;

struct ShapeSAT : public Shape {
	virtual std::vector<float3> getSeparatingAxes(TransformComponent transform) const = 0;
	virtual float2 getProjection(float3 axis, TransformComponent transform) const = 0;
};
} // namespace arch::physics