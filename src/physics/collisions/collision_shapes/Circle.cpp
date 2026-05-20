#include <archimedes/Mmath.h>
#include <archimedes/physics/Helpers.h>
#include <archimedes/physics/collisions/collision_shapes/Circle.h>

namespace arch::physics {
Circle::Circle(float2 center, f32 radius): center(center), radius(radius) {
	if (radius < COLLISIONS_EPSILON) {
		throw PhysicsException("Circle's radius must be greater than 0");
	}
}

float2 Circle::getSeparatingAxis(const TransformComponent& transform, const std::vector<float2>& realPolygonVertices)
	const {
	f32 minDistance = std::numeric_limits<f32>::max();
	float2 closestVertex;
	float2 realCenter = getRealCenter(transform);
	for (const auto& vertex : realPolygonVertices) {
		f32 distance = glm::distance2(vertex, realCenter);
		if (distance < minDistance) {
			minDistance = distance;
			closestVertex = vertex;
		}
	}
	float2 axis = closestVertex - realCenter;
	return glm::normalize(axis);
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

f32 Circle::getRealRadiusSquared(const TransformComponent& transform) const {
	f32 maxScalePart = glm::compMax(float2(transform.scale));
	return pow(maxScalePart * radius, 2);
}

f32 Circle::getRealRadius(const TransformComponent& transform) const {
	return std::sqrt(getRealRadiusSquared(transform));
}

bool Circle::containsPoint(const TransformComponent& transform, float2 point) const {
	float2 center = getRealCenter(transform);
	return glm::distance2(center, point) < getRealRadiusSquared(transform) + COLLISIONS_EPSILON;
}
} // namespace arch::physics
