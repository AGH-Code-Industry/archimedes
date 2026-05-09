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

float2 Circle::getProjection(const TransformComponent& transform, float3 axis) const {
	float3 realCenter = getRealCenter(transform);
	float centerProj = glm::dot(realCenter, axis);
	float radius = getRealRadius(transform);
	return float2{ centerProj - radius, centerProj + radius };
}

f32 Circle::getRealRadiusSquared(const TransformComponent& transform) const {
	return pow(glm::compMax(transform.scale) * radius, 2);
}

f32 Circle::getRealRadius(const TransformComponent& transform) const {
	return std::sqrt(getRealRadiusSquared(transform));
}

bool Circle::containsPoint(const TransformComponent& transform, float3 point) const {
	float3 center = getRealCenter(transform);
	return glm::distance2(center, point) < getRealRadiusSquared(transform) + 0.0001f;
}
} // namespace arch::physics
