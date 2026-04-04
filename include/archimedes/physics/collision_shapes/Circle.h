#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collision_shapes/SATShape.h>

namespace arch::physics {

struct Circle: public SATShape {
	f32 radius;
	float3 center;

	Circle(float3 center, f32 radius);
	float3 getSeparatingAxis(const TransformComponent& transform, const std::vector<float3>& realPolygonVertices) const;
	float2 getProjection(float3 axis, const TransformComponent& transform) const override;
	float3 getRealCenter(const TransformComponent& transform) const;
};
} // namespace arch::physics
