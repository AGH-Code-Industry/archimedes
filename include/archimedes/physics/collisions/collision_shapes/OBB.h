#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>

namespace arch::physics {

///@brief Rotated rectangle
struct OBB: public Shape {
	/// @brief Center of the rectangle
	float2 center;

	/// @brief Half-width and half-height of the rectangle
	float2 extents;

	/// @brief Rotation of the rectangle
	f32 rotation;

	/// @warning Both extent values must be greater than 0
	OBB(float2 center, float2 extents, f32 rotation);

	///@brief Used for SAT collision algorithm
	std::array<float2, 2> getSeparatingAxes(const TransformComponent& transform) const;
	float2 getProjection(const TransformComponent& transform, float2 axis) const;

	///@brief Used to cast the shape to map coordinates
	std::array<float2, 4> getRealVertices(const TransformComponent& transform) const;

	///@brief Used for mouse to shape collision checking
	bool containsPoint(const TransformComponent& transform, float2 point) const;
};
} // namespace arch::physics
