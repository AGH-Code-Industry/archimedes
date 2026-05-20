#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>

namespace arch::physics {

///@brief Simple circle shape
///@warning It might not work properly if object's scales on x and y axes are different
struct Circle: public Shape {
	f32 radius;
	float2 center;

	///@warning radius must be greater than 0
	Circle(float2 center, f32 radius);

	/// @brief Used for SAT collision algorithm
	float2 getSeparatingAxis(const TransformComponent& transform, const std::vector<float2>& realPolygonVertices) const;
	float2 getProjection(const TransformComponent& transform, float2 axis) const;

	///@brief Used to cast the shape to map coordinates
	float2 getRealCenter(const TransformComponent& transform) const;
	f32 getRealRadiusSquared(const TransformComponent& transform) const;
	f32 getRealRadius(const TransformComponent& transform) const;

	///@brief Used for mouse to shape collision checking
	bool containsPoint(const TransformComponent& transform, float2 point) const;
};
} // namespace arch::physics
