#include <gtest/gtest.h>
#include <physics/System.h>

TEST(Physics, TestUpdate) {
	using PhysicsSystem = arch::physics::System;
	EXPECT_THROW(PhysicsSystem::update(), std::runtime_error);
}
