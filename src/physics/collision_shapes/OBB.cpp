#include <archimedes/physics/collision_shapes/OBB.h>
#include <archimedes/Mmath.h>


namespace arch::physics {
	OBB::OBB(float3 topLeft, float3 bottomRight, f32 rotation) : topLeft(topLeft), bottomRight(bottomRight),
		rotation(rotation) {
		if (topLeft.x > bottomRight.x || topLeft.y < bottomRight.y) {
			throw PhysicsException("Corners' coordinates don't match, consider swapping them");
		}
	}

	float3 OBB::_getSeparatingAxis(float3 pairVertex1, float3 pairVertex2, float3 otherVertex) const {
		float3 edge = pairVertex2 - pairVertex1;
		float3 candidate1 = { -edge.y, edge.x, 0.0f };
		float3 candidate2 = -candidate1;
		if (glm::dot(candidate1, otherVertex - pairVertex1) > 0) {
			return candidate2;
		}
		return candidate1;
	}

	std::vector<float3> OBB::getSeparatingAxes(TransformComponent transform) const {
		std::vector<float3> vertices = getRealVertices(transform);
		std::vector<float3> axes;
		axes.push_back(_getSeparatingAxis(vertices[0], vertices[1], vertices[2]));
		axes.push_back(_getSeparatingAxis(vertices[1], vertices[2], vertices[3]));
		for (auto& axis : axes) {
			Quat quaternion = arch::quaternion(rotation);
			axis = quaternion * axis;
			axis = glm::normalize(axis);
		}
		return axes;
	}

	std::vector<float3> OBB::getRealVertices(TransformComponent transform) const {
		std::vector<float3> vertices = {
			topLeft,
			{ bottomRight.x, topLeft.y, 0.0f },
			bottomRight,
			{ topLeft.x, bottomRight.y, 0.0f }
		};
		Mat4x4 model = transform.getTransformMatrix();
		for (auto& vertex : vertices) {
			Quat quaternion = arch::quaternion(rotation);
			vertex = quaternion * vertex;
			vertex = float3(model * float4(vertex, 1.0f));
		}
		return vertices;
	}

	float2 OBB::getProjection(float3 axis, TransformComponent transform) const {
		std::vector<float3> vertices = getRealVertices(transform);
		f32 min = glm::dot(axis, vertices[0]);
		f32 max = min;
		for (i32 i=1; i<vertices.size(); i++) {
			f32 projection = glm::dot(axis, vertices[i]);
			if (projection < min) {
				min = projection;
			}
			else if (projection > max) {
				max = projection;
			}
		}
		return { min, max };
	}


} // namespace arch::physics
