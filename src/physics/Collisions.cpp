#include <archimedes/physics/Collisions.hpp>

namespace arch::physics {
	bool checkCollision(const AABB& lhs, const AABB& rhs) {
		if (lhs.topLeft.x <= rhs.topLeft.x && rhs.topLeft.x <= lhs.bottomRight.x) {
			// top left corner of right rectangle is in the left rectangle
			if (lhs.bottomRight.y <= rhs.topLeft.y && rhs.topLeft.y <= lhs.topLeft.y) {
				return true;
			}

			// bottom left corner of right rectangle is in the left rectangle
			if (lhs.bottomRight.y <= rhs.bottomRight.y && rhs.bottomRight.y <= lhs.topLeft.y) {
				return true;
			}
		}

		if (lhs.topLeft.x <= rhs.bottomRight.x && rhs.bottomRight.x <= lhs.bottomRight.x) {
			// top right corner of right rectangle is in the left rectangle
			if (lhs.bottomRight.y <= rhs.topLeft.y && rhs.topLeft.y <= lhs.topLeft.y) {
				return true;
			}

			// bottom right corner of right rectangle is in the left rectangle
			if (lhs.bottomRight.y <= rhs.bottomRight.y && rhs.bottomRight.y <= lhs.topLeft.y) {
				return true;
			}
		}

		return false;
	}
}