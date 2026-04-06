#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/SATShape.h>

namespace arch::physics {

struct OBB: public SATShape {
	float3 topLeft;
	float3 bottomRight;
	f32 rotation;

	OBB(float3 topLeft, float3 bottomRight, f32 rotation);
	std::vector<float3> getSeparatingAxes(const TransformComponent& transform) const;
	float2 getProjection(float3 axis, const TransformComponent& transform) const override;
	std::vector<float3> getRealVertices(const TransformComponent& transform) const;
};
} // namespace arch::physics
