#pragma once
#include <archimedes/physics/collision_shapes/Shape.h>
#include <archimedes/physics/PhysicsException.h>
#include <archimedes/scene/components/TransformComponent.h>

namespace arch::physics {
using TransformComponent = arch::scene::components::TransformComponent;

struct HorizontalLine : public Shape {
	f32 y;

	HorizontalLine(f32 y);
	f32 getRealPosition(const TransformComponent& transform) const;
};
} // namespace arch::physics