#include <archimedes/physics/Helpers.hpp>

#include <archimedes/Math.h>
#include <archimedes/physics/collisions/collision_shapes/OBB.h>

namespace arch::physics {
OBB::OBB(float2 center, float2 extents, f32 rotation): center(center), extents(extents), rotation(rotation) {
	if (extents.x < COLLISIONS_EPSILON || extents.y < COLLISIONS_EPSILON) {
		throw PhysicsException("OBB's extents' values should be positive");
	}
}

std::array<float2, 2> OBB::getSeparatingAxes(const TransformComponent& transform) const {
	std::array<float2, 4> vertices = getRealVertices(transform);
	Quat quaternion = arch::quaternion(rotation);
	// you don't need to check parallel axes with SAT
	std::array<float2, 2> axes = { getConvexPolygonNorm(vertices[0], vertices[1], vertices[2]),
								   getConvexPolygonNorm(vertices[1], vertices[2], vertices[3]) };
	for (auto& axis : axes) {
		axis = quaternion * float3(axis, 0.0f);
		axis = glm::normalize(axis);
	}
	return axes;
}

std::array<float2, 4> OBB::getRealVertices(const TransformComponent& transform) const {
	std::array<float2, 4> vertices = {
		float2{ center.x - extents.x, center.y + extents.y }, // top left
		float2{ center.x + extents.x, center.y + extents.y }, // top right
		float2{ center.x + extents.x, center.y - extents.y }, // bottom right
		float2{ center.x - extents.x, center.y - extents.y }, // bottom left
	};
	Mat4x4 model = transform.getTransformMatrix();
	Quat quaternion = arch::quaternion(rotation);
	for (auto& vertex : vertices) {
		vertex = quaternion * float3(vertex, 0.0f);
		vertex = model * float4(vertex, 0.0f, 1.0f);
	}
	return vertices;
}

float2 OBB::getProjection(const TransformComponent& transform, float2 axis) const {
	std::array<float2, 4> vertices = getRealVertices(transform);
	f32 min = glm::dot(axis, vertices[0]);
	f32 max = min;
	for (i32 i = 1; i < vertices.size(); i++) {
		f32 projection = glm::dot(axis, vertices[i]);
		if (projection < min) {
			min = projection;
		} else if (projection > max) {
			max = projection;
		}
	}
	return { min, max };
}

bool OBB::containsPoint(const TransformComponent& transform, float2 point) const {
	std::array<float2, 4> vertices = getRealVertices(transform);
	f32 areasSum = getSumOfTriangleAreas(vertices, point);
	f32 obbArea = getTriangleArea(vertices[0], vertices[1], vertices[2]) * 2.0f;
	return std::abs(areasSum - obbArea) < COLLISIONS_EPSILON;
}

} // namespace arch::physics
