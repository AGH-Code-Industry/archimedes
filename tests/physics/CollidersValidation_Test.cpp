#include <archimedes/physics/collisions/collision_shapes/Circle.h>
#include <archimedes/physics/collisions/collision_shapes/OBB.h>
#include <archimedes/physics/collisions/collision_shapes/Triangle.h>
#include <gtest/gtest.h>

namespace physics {
namespace ecs = arch::ecs;
namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

using TransformComponent = scene::components::TransformComponent;

class CollidersValidationTest: public ::testing::Test {};

///@brief Tests if the engine discards degenerate collision shapes

TEST_F(CollidersValidationTest, OBB_Degenerate_ZeroSize) {
	ASSERT_THROW(
		phy::OBB({
			{ 0.f, 0.f },
			{ 0.f, 0.f },
			0.f
	}),
		phy::PhysicsException
	);
}

TEST_F(CollidersValidationTest, OBB_Degenerate_InvalidExtents) {
	ASSERT_THROW(
		phy::OBB({
			{  1.f,  1.f },
			{ -1.f, -1.f },
			0.f
	}),
		phy::PhysicsException
	);
}

TEST_F(CollidersValidationTest, Circle_Degenerate_ZeroRadius) {
	ASSERT_THROW(
		phy::Circle({
			{ 0.f, 0.f },
			0.f
	}),
		phy::PhysicsException
	);
}

TEST_F(CollidersValidationTest, Circle_Degenerate_NegativeRadius) {
	ASSERT_THROW(
		phy::Circle({
			{ 0.f, 0.f },
			-1.f
	 }),
		phy::PhysicsException
	);
}

TEST_F(CollidersValidationTest, Triangle_Degenerate_AllPointsEqual) {
	ASSERT_THROW(
		phy::Triangle({
			{ 0.f, 0.f },
			{ 0.f, 0.f },
			{ 0.f, 0.f }
	 }),
		phy::PhysicsException
	);
}

TEST_F(CollidersValidationTest, Triangle_Degenerate_Collinear) {
	ASSERT_THROW(
		phy::Triangle({
			{ 0.f, 0.f },
			{ 1.f, 1.f },
			{ 2.f, 2.f }
	 }),
		phy::PhysicsException
	);
}

} // namespace physics
