#include <archimedes/Mmath.h>
#include <archimedes/physics/Helpers.h>
#include <archimedes/physics/collisions/collision_shapes/Triangle.h>

namespace arch::physics {
Triangle::Triangle(float2 pointA, float2 pointB, float2 pointC) {
	points[0] = pointA;
	points[1] = pointB;
	points[2] = pointC;
	if (getTriangleArea(pointA, pointB, pointC) < COLLISIONS_EPSILON) {
		throw PhysicsException("Triangle's area must be greater than 0");
	}
}

std::vector<float2> Triangle::getSeparatingAxes(const TransformComponent& transform) const {
	std::vector<float2> vertices = getRealVertices(transform);
	std::vector<float2> axes;
	for (i32 i = 0; i < 3; i++) {
		float2 axis = getConvexPolygonNorm(vertices[i], vertices[(i + 1) % 3], vertices[(i + 2) % 3]);
		axis = glm::normalize(axis);
		axes.push_back(axis);
	}
	return axes;
}

std::vector<float2> Triangle::getRealVertices(const TransformComponent& transform) const {
	std::vector<float2> vertices(points.begin(), points.end());
	Mat4x4 model = transform.getTransformMatrix();
	for (auto& vertex : vertices) {
		vertex = model * float4(vertex, 0.0f, 1.0f);
	}
	return vertices;
}

float2 Triangle::getProjection(const TransformComponent& transform, float2 axis) const {
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

bool Triangle::containsPoint(const TransformComponent& transform, float2 point) const {
	std::vector<float2> vertices = getRealVertices(transform);
	f32 areasSum = getSumOfTriangleAreas(vertices, point);
	f32 triangleArea = getTriangleArea(vertices[0], vertices[1], vertices[2]);
	return std::abs(areasSum - triangleArea) < COLLISIONS_EPSILON;
}

} // namespace arch::physics
