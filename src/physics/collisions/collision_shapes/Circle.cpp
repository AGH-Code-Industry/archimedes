#include <archimedes/physics/Helpers.hpp>
#include <archimedes/physics/collisions/collision_shapes/Circle.hpp>

#include <archimedes/Mmath.h>

namespace arch::physics {
Circle::Circle(float2 center, f32 radius): center(center), radius(radius) {
	if (radius < COLLISIONS_EPSILON) {
		throw PhysicsException("Circle's radius must be greater than 0");
	}
}

float2 Circle::getRealCenter(const TransformComponent& transform) const {
	Mat4x4 model = transform.getTransformMatrix();
	return float2(model * float4(center, 0.0f, 1.0f));
}

float2 Circle::getProjection(const TransformComponent& transform, float2 axis) const {
	float2 realCenter = getRealCenter(transform);
	f32 centerProj = glm::dot(realCenter, axis);
	f32 radius = getRealRadius(transform);
	return float2{ centerProj - radius, centerProj + radius };
}

f32 Circle::getRealRadius(const TransformComponent& transform) const {
	f32 maxScalePart = glm::compMax(float2(transform.scale));
	return maxScalePart * radius;
}

bool Circle::containsPoint(const TransformComponent& transform, float2 point) const {
	float2 center = getRealCenter(transform);
	return glm::distance(center, point) < getRealRadius(transform) + COLLISIONS_EPSILON;
}
} // namespace arch::physics
