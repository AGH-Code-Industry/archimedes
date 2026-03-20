#pragma once
#include "Shape.h"
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/scene/components/TransformComponent.h>


namespace arch::physics {

using TransformComponent = arch::scene::components::TransformComponent;

struct OBB : Shape {
	float3 topLeft;
	float3 bottomRight;
	f32 rotation;

	OBB(float3 topLeft, float3 bottomRight, f32 rotation);
	std::vector<float3> getSeparatingAxes(TransformComponent transform) const;
	float2 getProjection(float3 axis, TransformComponent transform) const;
	std::vector<float3> getRealVertices(TransformComponent transform) const;

private:
	float3 _getSeparatingAxis(float3 pairVertex1, float3 pairVertex2, float3 otherVertex) const;
};
} // namespace arch::physics