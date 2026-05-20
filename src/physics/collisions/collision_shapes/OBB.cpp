#include <archimedes/Mmath.h>
#include <archimedes/physics/Helpers.h>
#include <archimedes/physics/collisions/collision_shapes/OBB.h>

namespace arch::physics {
OBB::OBB(float2 topLeft, float2 bottomRight, f32 rotation):
	topLeft(topLeft),
	bottomRight(bottomRight),
	rotation(rotation) {
	if (topLeft.x > bottomRight.x || topLeft.y < bottomRight.y) {
		throw PhysicsException("OBB's corners' coordinates don't match, consider swapping them");
	}
	float2 bottomLeft = { topLeft.x, bottomRight.y };
	if (getTriangleArea(topLeft, bottomLeft, bottomRight) < COLLISIONS_EPSILON) {
		throw PhysicsException("OBB's area must be greater than 0");
	}
}

std::vector<float2> OBB::getSeparatingAxes(const TransformComponent& transform) const {
	std::vector<float2> vertices = getRealVertices(transform);
	Quat quaternion = arch::quaternion(rotation);
	std::vector<float2> axes;
	// you don't need to check parallel axes with SAT 
	axes.push_back(getConvexPolygonNorm(vertices[0], vertices[1], vertices[2]));
	axes.push_back(getConvexPolygonNorm(vertices[1], vertices[2], vertices[3]));
	for (auto& axis : axes) {
		axis = quaternion * float3(axis, 0.0f);
		axis = glm::normalize(axis);
	}
	return axes;
}

std::vector<float2> OBB::getRealVertices(const TransformComponent& transform) const {
	std::vector<float2> vertices = {
		topLeft,
		{ bottomRight.x, topLeft.y },
		bottomRight,
		{ topLeft.x, bottomRight.y }
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
	std::vector<float2> vertices = getRealVertices(transform);
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
	std::vector<float2> vertices = getRealVertices(transform);
	f32 areasSum = getSumOfTriangleAreas(vertices, point);
	f32 obbArea = getTriangleArea(vertices[0], vertices[1], vertices[2]) * 2.0f;
	return std::abs(areasSum - obbArea) < COLLISIONS_EPSILON;
}

} // namespace arch::physics
