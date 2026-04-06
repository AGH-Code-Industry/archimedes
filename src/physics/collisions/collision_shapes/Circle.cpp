#include <archimedes/Mmath.h>
#include <archimedes/physics/Helpers.h>
#include <archimedes/physics/collisions/collision_shapes/Circle.h>

namespace arch::physics {
Circle::Circle(float3 center, f32 radius): center(center), radius(radius) {}

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
	axis = { -axis.y, axis.x, 0.0f };
	return glm::normalize(axis);
}

float3 Circle::getRealCenter(const TransformComponent& transform) const {
	Mat4x4 model = transform.getTransformMatrix();
	return float3(model * float4(center, 1.0f));
}

float2 Circle::getProjection(float3 axis, const TransformComponent& transform) const {
	float3 realCenter = getRealCenter(transform);
	float3 lineVector = glm::normalize(float3{ axis.y, -axis.x, 0.0f });
	float3 pointA = realCenter + lineVector * radius;
	float3 pointB = realCenter - lineVector * radius;
	f32 dotA = glm::dot(axis, pointA);
	f32 dotB = glm::dot(axis, pointB);
	return dotA < dotB ? float2{ dotA, dotB } : float2{ dotB, dotA };
}

} // namespace arch::physics
