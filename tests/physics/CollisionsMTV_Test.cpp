#include <archimedes/physics/PhysicsSystem.h>
#include <archimedes/physics/collisions/ColliderComponent.h>
#include <archimedes/physics/components/RigidBodyComponent.h>
#include <archimedes/physics/Helpers.h>
#include <gtest/gtest.h>

using namespace arch::physics;

namespace physics {
namespace ecs = arch::ecs;
namespace phy = arch::physics;
namespace math = arch::math;
namespace scene = arch::scene;

using TransformComponent = scene::components::TransformComponent;

/// @brief Checks if the normal is close to another one (or its reversed version)
void expectNormalApprox(const math::float2& actual, const math::float2& expected, float eps = 1e-4f) {
	bool same = std::abs(actual.x - expected.x) < eps && std::abs(actual.y - expected.y) < eps;

	bool opposite = std::abs(actual.x + expected.x) < eps && std::abs(actual.y + expected.y) < eps;

	EXPECT_TRUE(same || opposite);
}

/// @brief Tests calculating penetration vectors and whether they really allow to remove collisions
class CollisionsMTVTest: public ::testing::Test {
protected:
	void SetUp() override {
		_domain = std::make_unique<ecs::Domain>();
		_system = std::make_unique<phy::PhysicsSystem>(*_domain);
	}

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

TEST_F(CollisionsMTVTest, CircleVsCircle_DepthNormal) {
	phy::Circle circle{
		{ 0, 0 },
		0.5f
	};

	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 0.8f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto result = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	ASSERT_TRUE(result);
	const auto& col = result.value();

	EXPECT_NEAR(col.depth, 0.2f, 1e-4f);
	expectNormalApprox(col.normal, { 1.f, 0.f });
}

TEST_F(CollisionsMTVTest, OBBvsOBB_DepthNormal) {
	phy::OBB obb{
		{ -0.5f, 0.5f },
		{  0.5f, -0.5f },
		0.f
	};

	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);
	auto e2 = createOBB({ 0.8f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	auto result = phy::ColliderComponent::areColliding(
		_domain->getComponent<phy::ColliderComponent>(e1),
		_domain->getComponent<phy::ColliderComponent>(e2),
		_domain->getComponent<TransformComponent>(e1),
		_domain->getComponent<TransformComponent>(e2)
	);

	ASSERT_TRUE(result);
	const auto& col = result.value();

	EXPECT_NEAR(col.depth, 0.2f, 1e-4f);
	expectNormalApprox(col.normal, { 1.f, 0.f });
}

TEST_F(CollisionsMTVTest, CircleVsCircle_ResolveCollision) {
	phy::Circle circle{
		{ 0, 0 },
		0.5f
	};

	arch::float2 firstCircleSpeed = { 0.1f, 0 };
	auto e1 = createCircle({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);
	auto e2 = createCircle({ 0.8f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, circle);

	auto& col1 = _domain->getComponent<phy::ColliderComponent>(e1);
	auto& col2 = _domain->getComponent<phy::ColliderComponent>(e2);
	auto& t1 = _domain->getComponent<TransformComponent>(e1);
	auto& t2 = _domain->getComponent<TransformComponent>(e2);

	auto result = phy::ColliderComponent::areColliding(col1, col2, t1, t2);
	ASSERT_TRUE(result);

	auto col = result.value();

	auto correction = math::float3(col.normal * (col.depth + 0.001f), 0.0f);

	if (areVectorsSameDirection(firstCircleSpeed, correction)) {
		correction = -correction;
	}

	t1.position += correction;

	auto resultAfter = phy::ColliderComponent::areColliding(col1, col2, t1, t2);
	EXPECT_FALSE(resultAfter.has_value());
}

TEST_F(CollisionsMTVTest, OBBvsOBB_ResolveCollision_Split) {
	phy::OBB obb{
		{ -0.5f, 0.5f },
		{  0.5f, -0.5f },
		0.f
	};

	auto e1 = createOBB({ 0, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);
	auto e2 = createOBB({ 0.8f, 0, 0 }, { 0, 0, 0, 1 }, { 1, 1, 1 }, obb);

	auto& col1 = _domain->getComponent<phy::ColliderComponent>(e1);
	auto& col2 = _domain->getComponent<phy::ColliderComponent>(e2);
	auto& t1 = _domain->getComponent<TransformComponent>(e1);
	auto& t2 = _domain->getComponent<TransformComponent>(e2);

	auto result = phy::ColliderComponent::areColliding(col1, col2, t1, t2);
	ASSERT_TRUE(result);

	auto col = result.value();

	auto correction = math::float3(col.normal * (col.depth + 0.001f), 0.f);

	t1.position += correction * 0.5f;
	t2.position -= correction * 0.5f;

	auto resultAfter = phy::ColliderComponent::areColliding(col1, col2, t1, t2);
	EXPECT_FALSE(resultAfter.has_value());
}

} // namespace physics
