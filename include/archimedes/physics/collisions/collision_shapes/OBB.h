#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>

namespace arch::physics {

///@brief Rotated rectangle
struct OBB: public Shape {
	float2 topLeft;
	float2 bottomRight;
	f32 rotation;

	///@warning Coordinates should match, also area should be greater than 0
	OBB(float2 topLeft, float2 bottomRight, f32 rotation);

	///@brief Used for SAT collision algorithm
	std::vector<float2> getSeparatingAxes(const TransformComponent& transform) const;
	float2 getProjection(const TransformComponent& transform, float2 axis) const;

	///@brief Used to cast the shape to map coordinates
	std::vector<float2> getRealVertices(const TransformComponent& transform) const;

	///@brief Used for mouse to shape collision checking
	bool containsPoint(const TransformComponent& transform, float2 point) const;
};
} // namespace arch::physics
