#include <archimedes/physics/Collisions.hpp>

namespace arch::physics {

	bool areProjectionsOverlapping(float2 projection1, float2 projection2) {
		return !(projection1.y < projection2.x || projection2.y < projection1.x);
	}

	bool checkSAT(const std::vector<float3>& axes1, const std::vector<float3>& axes2,
				const TransformComponent& transform1, const TransformComponent& transform2,
				const ShapeSAT& shape1, const ShapeSAT& shape2){
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

}