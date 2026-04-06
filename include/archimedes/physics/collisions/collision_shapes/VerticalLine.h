#pragma once
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/physics/collisions/collision_shapes/Shape.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {
using TransformComponent = arch::scene::components::TransformComponent;

struct VerticalLine: public Shape {
	f32 x;

	VerticalLine(f32 x);
	f32 getRealPosition(const TransformComponent& transform) const;
};
} // namespace arch::physics
