#include <archimedes/Mmath.h>
#include <archimedes/physics/Helpers.h>
#include <archimedes/physics/collision_shapes/OBB.h>

namespace arch::physics {
OBB::OBB(float3 topLeft, float3 bottomRight, f32 rotation):
	topLeft(topLeft),
	bottomRight(bottomRight),
	rotation(rotation) {
	if (topLeft.x > bottomRight.x || topLeft.y < bottomRight.y) {
		throw PhysicsException("Corners' coordinates don't match, consider swapping them");
	}
}

std::vector<float3> OBB::getSeparatingAxes(const TransformComponent& transform) const {
	std::vector<float3> vertices = getRealVertices(transform);
	Quat quaternion = arch::quaternion(rotation);
	std::vector<float3> axes;
	axes.push_back(getConvexPolygonNorm(vertices[0], vertices[1], vertices[2]));
	axes.push_back(getConvexPolygonNorm(vertices[1], vertices[2], vertices[3]));
	for (auto& axis : axes) {
		axis = quaternion * axis;
		axis = glm::normalize(axis);
	}
	return axes;
}

std::vector<float3> OBB::getRealVertices(const TransformComponent& transform) const {
	std::vector<float3> vertices = {
		topLeft,
		{ bottomRight.x,	 topLeft.y, 0.0f },
		bottomRight,
		{	  topLeft.x, bottomRight.y, 0.0f }
	};
	Mat4x4 model = transform.getTransformMatrix();
	Quat quaternion = arch::quaternion(rotation);
	for (auto& vertex : vertices) {
		vertex = quaternion * vertex;
		vertex = float3(model * float4(vertex, 1.0f));
	}
	return vertices;
}

float2 OBB::getProjection(float3 axis, const TransformComponent& transform) const {
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
