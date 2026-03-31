#include <archimedes/physics/components/RigidBodyComponent.h>
#include <archimedes/physics/components/ColliderComponent.h>
#include <archimedes/Scene.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <gtest/gtest.h>

namespace physics {
namespace ecs = arch::ecs;
namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

TEST(Physics, TestUpdate) {
	ecs::Domain domain;
	phy::PhysicsSystem system(domain);

	constexpr auto startV = math::float3(1.f, 0.f, 0.f);

	// Create movable entity
	auto e0 = domain.newEntity();
	auto& rigidBody = domain.addComponent<phy::RigidBodyComponent>(
		e0,
		{
			1.f,
			{1.f, 1.f, 0.f},
			startV,
			0.0f
		}
	);

	auto& transform = domain.addComponent<scene::components::TransformComponent>(
		e0,
		{
			{0.f, 0.f, 0.f},
			{0.f, 0.f, 0.f, 1.f},
			math::float3(1)
		}
	);

	auto collision = [&, &domain = domain](const ecs::Entity me, const ecs::Entity other) {};


	domain.addComponent(e0,
		phy::ColliderComponent{
			.shape = phy::OBB (
					math::float3{-0.25f, 0.25f, 0.0f},
					math::float3{ 0.25f, -0.25f , 0.0f},
					0.0f
			),
			.action = collision,
		}
	);

	// Update physics system
	const math::f32 t = system.update();

	// Expect entity to move accordingly to the old velocity
	EXPECT_EQ(transform.position, t * startV);

	// Expect that velocity updated accordingly to the force
	EXPECT_EQ(rigidBody.linearVelocity, startV + t * rigidBody.force);
}

} // namespace physics
