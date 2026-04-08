#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/SATShape.h>

namespace arch::physics {

/// @brief Triangle collision shape
struct Triangle: public SATShape {
	std::array<float3, 3> points;

	///@warning Area should be greater than 0
	Triangle(float3 pointA, float3 pointB, float3 pointC);

	///@brief Used for SAT collision algorithm
	std::vector<float3> getSeparatingAxes(const TransformComponent& transform) const;
	float2 getProjection(float3 axis, const TransformComponent& transform) const override;

	///@brief Used to cast the shape to the map coordinates
	std::vector<float3> getRealVertices(const TransformComponent& transform) const;
};
} // namespace arch::physics
