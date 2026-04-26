#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>

namespace arch::physics {

///@brief Rotated rectangle
struct OBB: public Shape {
	float3 topLeft;
	float3 bottomRight;
	f32 rotation;

	///@warning Coordinates should match, also area should be greater than 0
	OBB(float3 topLeft, float3 bottomRight, f32 rotation);

	///@brief Used for SAT collision algorithm
	std::vector<float3> getSeparatingAxes(const TransformComponent& transform) const;
	float2 getProjection(float3 axis, const TransformComponent& transform) const override;

	///@brief Used to cast the shape to map coordinates
	std::vector<float3> getRealVertices(const TransformComponent& transform) const;
};
} // namespace arch::physics
