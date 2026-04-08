#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/SATShape.h>

namespace arch::physics {

///@brief Simple circle shape
///@warning It might not work properly if object's scales on x and y axes are different
struct Circle: public SATShape {
	f32 radius;
	float3 center;

	///@warning radius must be greater than 0
	Circle(float3 center, f32 radius);

	/// @brief Used for SAT collision algorithm
	float3 getSeparatingAxis(const TransformComponent& transform, const std::vector<float3>& realPolygonVertices) const;
	float2 getProjection(float3 axis, const TransformComponent& transform) const override;

	///@brief Used to cast the shape to map coordinates
	float3 getRealCenter(const TransformComponent& transform) const;
	f32 getRealRadiusSquared(const TransformComponent& transform) const;
	f32 getRealRadius(const TransformComponent& transform) const;
};
} // namespace arch::physics
