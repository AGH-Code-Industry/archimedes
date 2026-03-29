#pragma once
#include <archimedes/physics/collision_shapes/ShapeSAT.h>
#include <archimedes/physics/PhysicsException.h>

namespace arch::physics {

struct Triangle : public ShapeSAT {
    std::array<float3, 3> points;

	Triangle(float3 pointA, float3 pointB, float3 pointC);
	std::vector<float3> getSeparatingAxes(TransformComponent transform) const override;
	float2 getProjection(float3 axis, TransformComponent transform) const override;
	std::vector<float3> getRealVertices(TransformComponent transform) const;
};
} // namespace arch::physics