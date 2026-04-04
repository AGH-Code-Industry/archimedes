#include <archimedes/physics/Collisions.hpp>

namespace arch::physics {

	bool areProjectionsOverlapping(float2 projection1, float2 projection2) {
		return !(projection1.y < projection2.x || projection2.y < projection1.x);
	}

	bool checkSAT(const std::vector<float3>& axes1, const std::vector<float3>& axes2,
				const TransformComponent& transform1, const TransformComponent& transform2,
				const SATShape& shape1, const SATShape& shape2){
		for (auto& axis : axes1) {
			float2 projection1 = shape1.getProjection(axis, transform1);
			float2 projection2 = shape2.getProjection(axis, transform2);
			if (!areProjectionsOverlapping(projection1, projection2)) {
				return false;
			}
		}
		for (auto& axis : axes2) {
			float2 projection1 = shape1.getProjection(axis, transform1);
			float2 projection2 = shape2.getProjection(axis, transform2);
			if (!areProjectionsOverlapping(projection1, projection2)) {
				return false;
			}
		}
		return true;
	}

	bool checkCollision(const OBB& shape1, const OBB& shape2,
		const TransformComponent& transform1, const TransformComponent& transform2) {
		std::vector<float3> axes1 = shape1.getSeparatingAxes(transform1);
		std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
		return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
	}

	bool checkCollision(const Triangle& shape1, const Triangle& shape2,
		const TransformComponent& transform1, const TransformComponent& transform2) {
		std::vector<float3> axes1 = shape1.getSeparatingAxes(transform1);
		std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
		return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
	}

	bool checkCollision(const Triangle& shape1, const OBB& shape2,
		const TransformComponent& transform1, const TransformComponent& transform2) {
		std::vector<float3> axes1 = shape1.getSeparatingAxes(transform1);
		std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
		return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
	}

	bool checkCollision(const Circle& shape1, const Circle& shape2,
		const TransformComponent& transform1, const TransformComponent& transform2) {
		float3 center1 = shape1.getRealCenter(transform1);
		float3 center2 = shape2.getRealCenter(transform2);
		f32 distanceSquared = glm::dot(center1 - center2, center1 - center2);
		f32 radiusSum = shape1.radius + shape2.radius;
		return distanceSquared < radiusSum * radiusSum;
	}

	bool checkCollision(const Circle& shape1, const Triangle& shape2,
		const TransformComponent& transform1, const TransformComponent& transform2) {
		std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
		float3 axis1 = shape1.getSeparatingAxis(transform1, shape2.getRealVertices(transform2));
		std::vector<float3> axes1 = { axis1 };
		return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
	}

	bool checkCollision(const Circle& shape1, const OBB& shape2,
		const TransformComponent& transform1, const TransformComponent& transform2) {
		std::vector<float3> axes2 = shape2.getSeparatingAxes(transform2);
		float3 axis1 = shape1.getSeparatingAxis(transform1, shape2.getRealVertices(transform2));
		std::vector<float3> axes1 = { axis1 };
		return checkSAT(axes1, axes2, transform1, transform2, shape1, shape2);
	}

}