#pragma once

#include <chrono>

#include "Mmath.h"
#include "ecs/Domain.h"

namespace arch::physics {

class System {
public:
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
	using Duration = std::chrono::duration<f32>;

	explicit System(ecs::Domain& domain);
	f32 update();

private:
	ecs::Domain& _domain;
	TimePoint _prevTimePoint;

	void _collisionDetection(f32 t) const;
};

} // namespace arch::physics
