#include <archimedes/Mmath.h>
#include <archimedes/physics/Helpers.h>
#include <archimedes/physics/collisions/collision_shapes/Circle.h>

namespace arch::physics {
Circle::Circle(float3 center, f32 radius): center(center), radius(radius) {
	if (radius < 0.0001f) {
		throw PhysicsException("Circle's radius must be greater than 0");
	}
}

float3 Circle::getSeparatingAxis(const TransformComponent& transform, const std::vector<float3>& realPolygonVertices)
	const {
	f64 minDistance = std::numeric_limits<f64>::max();
	float3 closestVertex;
	float3 realCenter = getRealCenter(transform);
	for (const auto& vertex : realPolygonVertices) {
		f64 distance = glm::distance2(vertex, realCenter);
		if (distance < minDistance) {
			minDistance = distance;
			closestVertex = vertex;
		}
	}
	float3 axis = closestVertex - realCenter;
	return glm::normalize(axis);
}

float3 Circle::getRealCenter(const TransformComponent& transform) const {
	Mat4x4 model = transform.getTransformMatrix();
	return float3(model * float4(center, 1.0f));
}

float2 Circle::getProjection(float3 axis, const TransformComponent& transform) const {
	float3 realCenter = getRealCenter(transform);
	float centerProj = glm::dot(realCenter, axis);
	float radius = getRealRadius(transform);
	return float2{ centerProj - radius, centerProj + radius };
}

f32 Circle::getRealRadiusSquared(const TransformComponent& transform) const {
	Mat4x4 model = transform.getTransformMatrix();
	float3 radiusVector = float3{ radius, 0.0f, 0.0f };
	radiusVector = float3(model * float4(radiusVector, 0.0f));
	return glm::length2(radiusVector);
}

f32 Circle::getRealRadius(const TransformComponent& transform) const {
	return std::sqrt(getRealRadiusSquared(transform));
}
} // namespace arch::physics
