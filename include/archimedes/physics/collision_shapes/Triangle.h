#pragma once
#include <archimedes/physics/collision_shapes/SATShape.h>
#include <archimedes/physics/PhysicsException.h>

namespace arch::physics {

struct Triangle : public SATShape {
    std::array<float3, 3> points;

	Triangle(float3 pointA, float3 pointB, float3 pointC);
	std::vector<float3> getSeparatingAxes(const TransformComponent& transform) const;
	float2 getProjection(float3 axis, const TransformComponent& transform) const override;
	std::vector<float3> getRealVertices(const TransformComponent& transform) const;
};
} // namespace arch::physics