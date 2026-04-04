#include <archimedes/Mmath.h>
#include <archimedes/physics/Helpers.h>
#include <archimedes/physics/collision_shapes/Triangle.h>

namespace arch::physics {
Triangle::Triangle(float3 pointA, float3 pointB, float3 pointC) {
	points[0] = pointA;
	points[1] = pointB;
	points[2] = pointC;
}

std::vector<float3> Triangle::getSeparatingAxes(const TransformComponent& transform) const {
	std::vector<float3> vertices = getRealVertices(transform);
	std::vector<float3> axes;
	for (i32 i = 0; i < 3; i++) {
		float3 axis = getConvexPolygonNorm(vertices[i], vertices[(i + 1) % 3], vertices[(i + 2) % 3]);
		axis = glm::normalize(axis);
		axes.push_back(axis);
	}
	return axes;
}

std::vector<float3> Triangle::getRealVertices(const TransformComponent& transform) const {
	std::vector<float3> vertices(points.begin(), points.end());
	Mat4x4 model = transform.getTransformMatrix();
	for (auto& vertex : vertices) {
		vertex = float3(model * float4(vertex, 1.0f));
	}
	return vertices;
}

float2 Triangle::getProjection(float3 axis, const TransformComponent& transform) const {
	std::vector<float3> vertices = getRealVertices(transform);
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

} // namespace arch::physics
