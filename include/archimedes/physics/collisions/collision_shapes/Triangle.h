#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>

namespace arch::physics {

/// @brief Triangle collision shape
struct Triangle: public Shape {
	std::array<float2, 3> points;

	///@warning Area should be greater than 0
	Triangle(float2 pointA, float2 pointB, float2 pointC);

	///@brief Used for SAT collision algorithm
	std::vector<float2> getSeparatingAxes(const TransformComponent& transform) const;
	float2 getProjection(const TransformComponent& transform, float2 axis) const;

	///@brief Used to cast the shape to the map coordinates
	std::vector<float2> getRealVertices(const TransformComponent& transform) const;

	///@brief Used for mouse to shape collision checking
	bool containsPoint(const TransformComponent& transform, float2 point) const;
};
} // namespace arch::physics
