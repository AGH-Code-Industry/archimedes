#include <GLFW/glfw3.h>
#include <archimedes/Scene.h>
#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <gtest/gtest.h>

namespace physics {
namespace ecs = arch::ecs;
namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

using TransformComponent = scene::components::TransformComponent;

///@brief Tests shape-shape collision algorithms and collision layers
class CollisionTest: public ::testing::Test {
protected:
	void SetUp() override {
		if (!glfwInit()) {
			FAIL() << "Failed to initialize GLFW";
		}
		_domain = std::make_unique<ecs::Domain>();
		_system = std::make_unique<phy::PhysicsSystem>(*_domain);
	}

	void TearDown() override { glfwTerminate(); }

	ecs::Entity createOBB(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::OBB& obb
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = obb });
		return e;
	}

	ecs::Entity createTriangle(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::Triangle& tri
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = tri });
		return e;
	}

	ecs::Entity createCircle(
		const math::float3& pos,
		const math::Quat& rot,
		const math::float3& scale,
		const phy::Circle& circle
	) {
		auto e = _domain->newEntity();
		_domain->addComponent<TransformComponent>(e, { pos, rot, scale });
		_domain->addComponent<phy::ColliderComponent>(e, phy::ColliderComponent{ .shape = circle });
		return e;
	}

	std::unique_ptr<ecs::Domain> _domain;
	std::unique_ptr<phy::PhysicsSystem> _system;
};

TEST_F(CollisionTest, OBBvsOBB_Collision) {
	phy::OBB obb{
		{ -0.25f,  0.25f },
		{  0.25f, -0.25f },
		0.f
	};
	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);
	auto e2 = createOBB({ 0.2f, 0.2f, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, OBBvsOBB_TouchingEdges) {
	phy::OBB obb{
		{ -0.25f,  0.25f },
		{  0.25f, -0.25f },
		0.f
	};
	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);
	auto e2 = createOBB({ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, OBBvsOBB_RotatedCollision) {
	phy::OBB obb1{
		{ -0.25f,  0.25f },
		{  0.25f, -0.25f },
		0.f
	};
	phy::OBB obb2{
		{ -0.25f,  0.25f },
		{  0.25f, -0.25f },
		0.785398f
	}; // ~45 deg

	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb1);
	auto e2 = createOBB({ 0.3f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb2);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, CircleVsCircle_Collision) {
	phy::Circle circle{
		{ 0, 0 },
		0.4f
	};
	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, CircleVsCircle_NoCollision) {
	phy::Circle circle{
		{ 0, 0 },
		0.4f
	};
	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 1.0f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsTriangle_NoCollision) {
	phy::Triangle tri{
		{  0.0f,	0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};
	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createTriangle({ 2, 2, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsTriangle_Collision) {
	phy::Triangle tri{
		{  0.0f,	0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};
	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createTriangle({ 0.2f, 0.1f, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsTriangle_Touching) {
	phy::Triangle tri{
		{  0.0f,	0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};
	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createTriangle({ 1, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsCircle_NoCollision) {
	phy::Triangle tri{
		{  0.0f,	0.5f },
		{ -0.5f, -0.5f },
		{  0.5f, -0.5f }
	};
	phy::Circle circle{
		{ 0.0f, 0.0f },
		0.3f
	};

	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createCircle({ 2, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, TriangleVsOBB_Collision) {
	phy::Triangle tri{
		{
			0.0f, 0.5f,
		 },
		{ -0.5f, -0.5f },
		{ 0.5f, -0.5f }
	};
	phy::OBB obb{
		{ -0.25f,  0.25f },
		{  0.25f, -0.25f },
		0.f
	};

	auto e1 = createTriangle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, tri);
	auto e2 = createOBB({ 0.2f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, Mask_NoMatch) {
	phy::Circle circle{
		{ 0.0f, 0.0f },
		1.0f
	};

	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto& c1 = _domain->getComponent<phy::ColliderComponent>(e1);
	auto& c2 = _domain->getComponent<phy::ColliderComponent>(e2);

	c1.isScannedMask = std::bitset<32>(1 << 0);
	c1.scansMask = std::bitset<32>(1 << 0);

	c2.isScannedMask = std::bitset<32>(1 << 1);
	c2.scansMask = std::bitset<32>(1 << 1);

	_system->update();

	ASSERT_FALSE(_system->getCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getCollisions(e2).contains(e1));
}

TEST_F(CollisionTest, Mask_OneWayMatch) {
	phy::Circle circle{
		{ 0.0f, 0.0f },
		1.0f
	};

	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto& c1 = _domain->getComponent<phy::ColliderComponent>(e1);
	auto& c2 = _domain->getComponent<phy::ColliderComponent>(e2);

	c1.isScannedMask = std::bitset<32>(0);
	c1.scansMask = std::bitset<32>(1 << 1);

	c2.isScannedMask = std::bitset<32>(1 << 1);
	c2.scansMask = std::bitset<32>(0);

	_system->update();
	ASSERT_TRUE(_system->getCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getCollisions(e2).contains(e1));
}

TEST_F(CollisionTest, Mask_MultipleLayers) {
	phy::Circle circle{
		{ 0.0f, 0.0f },
		1.0f
	};

	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 0.5f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto& c1 = _domain->getComponent<phy::ColliderComponent>(e1);
	auto& c2 = _domain->getComponent<phy::ColliderComponent>(e2);

	c1.isScannedMask = std::bitset<32>(0);
	c1.scansMask = std::bitset<32>(3);

	c2.isScannedMask = std::bitset<32>(1);
	c2.scansMask = std::bitset<32>(0);

	_system->update();
	ASSERT_TRUE(_system->getCollisions(e1).contains(e2));
}

TEST_F(CollisionTest, CircleVsCircle_BoundingCirclePreventsFalseNegative) {
	phy::Circle circle{
		{ 0.0f, 0.0f },
		0.5f
	};

	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 3.0f, 1.0f, 1.0f }, circle);

	auto e2 = createCircle({ 1.9f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, CircleVsCircle_NonUniformScale_ShouldNotCollide) {
	phy::Circle circle{
		{ 0.0f, 0.0f },
		0.5f
	};

	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 2.0f, 1.0f, 1.0f }, circle);

	auto e2 = createCircle({ 1.6f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_FALSE(phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	));
}

TEST_F(CollisionTest, StatesCheck) {
	phy::Circle circle{
		{ 0.0f, 0.0f },
		1.0f
	};
	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 3.0f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto& transform1 = _domain->getComponent<TransformComponent>(e1);

	ASSERT_FALSE(_system->getEnteredCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getExitedCollisions(e1).contains(e2));

	transform1.position.x += 2.0f;
	_system->update();
	ASSERT_TRUE(_system->getEnteredCollisions(e1).contains(e2));
	ASSERT_TRUE(_system->getCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getExitedCollisions(e1).contains(e2));

	_system->update();
	ASSERT_FALSE(_system->getEnteredCollisions(e1).contains(e2));
	ASSERT_TRUE(_system->getCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getExitedCollisions(e1).contains(e2));

	transform1.position.x -= 2.0f;
	_system->update();
	ASSERT_FALSE(_system->getEnteredCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getCollisions(e1).contains(e2));
	ASSERT_TRUE(_system->getExitedCollisions(e1).contains(e2));

	_system->update();
	ASSERT_FALSE(_system->getEnteredCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getCollisions(e1).contains(e2));
	ASSERT_FALSE(_system->getExitedCollisions(e1).contains(e2));
}

TEST_F(CollisionTest, CircleVsCircle_DifferentZ_ShouldStillCollide) {
	phy::Circle circle{
		{ 0.0f, 0.0f },
		0.5f
	};

	auto e1 = createCircle({ 0, 0, -100.0f }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto e2 = createCircle({ 0.4f, 0, 250.0f }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
					_domain->getComponent<phy::ColliderComponent>(e1),
					_domain->getComponent<phy::ColliderComponent>(e2),
					_domain->getComponent<TransformComponent>(e1),
					_domain->getComponent<TransformComponent>(e2)
	)
					.has_value());
}

TEST_F(CollisionTest, OBBvsOBB_DifferentZ_ShouldStillCollide) {
	phy::OBB obb{
		{ -0.5f,	 0.5f },
		{  0.5f, -0.5f },
		0.f
	};

	auto e1 = createOBB({ 0, 0, -50.0f }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	auto e2 = createOBB({ 0.25f, 0.25f, 999.0f }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	ASSERT_TRUE(phy::ColliderComponent::areColliding(
					_domain->getComponent<phy::ColliderComponent>(e1),
					_domain->getComponent<phy::ColliderComponent>(e2),
					_domain->getComponent<TransformComponent>(e1),
					_domain->getComponent<TransformComponent>(e2)
	)
					.has_value());
}

TEST_F(CollisionTest, CollisionDependsOnlyOnXY) {
	phy::Circle circle{
		{ 0.0f, 0.0f },
		0.5f
	};

	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto e2 = createCircle({ 0.75f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto collisionAtZ0 = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	_domain->getComponent<TransformComponent>(e2).position.z = 5000.0f;

	auto collisionAtZ5000 = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	ASSERT_EQ(collisionAtZ0.has_value(), collisionAtZ5000.has_value());
	ASSERT_TRUE(collisionAtZ5000.has_value());
}

} // namespace physics
