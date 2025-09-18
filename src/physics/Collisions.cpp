#include <archimedes/physics/Collisions.hpp>

namespace arch::physics {
	bool checkCollision(const AABB& shape1, const AABB& shape2,
		const TransformComponent& transform1, const TransformComponent& transform2) {

		auto topLeft1 = shape1.topLeft + transform1.position;
		auto bottomRight1 = shape1.bottomRight + transform1.position;

		auto topLeft2 = shape2.topLeft + transform2.position;
		auto bottomRight2 = shape2.bottomRight + transform2.position;

		if (topLeft1.x <= topLeft2.x && topLeft2.x <= bottomRight1.x) {
			// top left corner of right rectangle is in the left rectangle
			if (bottomRight1.y <= topLeft2.y && topLeft2.y <= topLeft1.y) {
				return true;
			}

			// bottom left corner of right rectangle is in the left rectangle
			if (bottomRight1.y <= bottomRight2.y && bottomRight2.y <= topLeft1.y) {
				return true;
			}
		}

		if (topLeft1.x <= bottomRight2.x && bottomRight2.x <= bottomRight1.x) {
			// top right corner of right rectangle is in the left rectangle
			if (bottomRight1.y <= topLeft2.y && topLeft2.y <= topLeft1.y) {
				return true;
			}

			// bottom right corner of right rectangle is in the left rectangle
			if (bottomRight1.y <= bottomRight2.y && bottomRight2.y <= topLeft1.y) {
				return true;
			}
		}

		return false;
	}
}