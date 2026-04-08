#pragma once

namespace arch::physics {
///@brief Shape that can collide with other shapes
struct Shape {
	virtual ~Shape() = default;
};
} // namespace arch::physics
