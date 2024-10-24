#include <Ecs.h>
#include <gtest/gtest.h>

namespace ecs = arch::ecs;
using Traits = ecs::_details::EntityTraits;

TEST(ECS, EntityCreationAlive) {
	// e64 - 64-bit entity handle
	// there also exists e32, with smaller range
	ecs::Domain domain;

	for (size_t i = 0; i != 100; ++i) {
		auto e = domain.newEntity();

		// entities' ids start from 0
		EXPECT_EQ(Traits::Id::part(e), i);
		// entities' versions start from 0
		EXPECT_EQ(Traits::Version::part(e), 0);

		EXPECT_TRUE(domain.alive(e));
	}
}

TEST(ECS, EntityKillAlive) {
	ecs::Domain domain;
	std::array<ecs::Entity, 10> toDelete;

	for (size_t i = 0; i != 100; ++i) {
		auto e = domain.newEntity();

		// mark some entites for deletion
		if (i % 10 == 0) {
			toDelete[i / 10] = e;
		}

		// entities' ids start from 0
		EXPECT_EQ(Traits::Id::part(e), i);
		// entities' versions start from 0
		EXPECT_EQ(Traits::Version::part(e), 0);

		EXPECT_TRUE(domain.alive(e));
	}
	for (auto&& e : toDelete) {
		domain.kill(e);

		// killed entities are not alive
		EXPECT_FALSE(domain.alive(e));
	}

	// discouraged usage vvv
	// EXPECT_FALSE(domain.alive(Traits::Entity::fromParts(10, 0)));
	// only handles aquired by interactions with domain are guaranteed to give valid results
	// but since its C++ you can do whatever you want
}

TEST(ECS, EntityRecycle) {
	ecs::Domain domain;

	// 10 entites
	for (size_t i = 0; i != 10; ++i) {
		auto e = domain.newEntity();
	}

	// 10 entites, but marked for deletion
	std::array<ecs::Entity, 10> toDelete;
	for (size_t i = 0; i != 10; ++i) {
		toDelete[i] = domain.newEntity();
	}

	// 10 entites
	for (size_t i = 0; i != 10; ++i) {
		auto e = domain.newEntity();
	}

	// 30 entities total

	for (auto&& td : toDelete) {
		domain.kill(td);

		EXPECT_FALSE(domain.alive(td));
	}

	// recycle entities in reverse order
	for (auto&& toRecycle : std::views::reverse(toDelete)) {
		auto recycled = domain.recycleEntity(toRecycle);

		// recycling entities by handle preserves version
		EXPECT_EQ(recycled, toRecycle);

		// meaning both recycled and toRecycle are equal
		EXPECT_TRUE(domain.alive(recycled));
		EXPECT_TRUE(domain.alive(toRecycle));
	}
	for (auto&& td : toDelete) {
		domain.kill(td);

		EXPECT_FALSE(domain.alive(td));
	}

	for (auto&& toAdd : std::views::reverse(toDelete)) {
		// recycling entities' ids
		auto recycled = domain.recycleId(Traits::Id::part(toAdd));

		// recycling ids does not preserve version, its incremented
		EXPECT_EQ(Traits::Version::part(recycled), 1);

		// that's why recycled is alive, toRecycle is not
		EXPECT_TRUE(domain.alive(recycled));
		EXPECT_FALSE(domain.alive(toAdd));
	}
}

TEST(ECS, EntityRecycleExisting) {
	ecs::Domain domain;

	// 10 entites
	for (size_t i = 0; i != 10; ++i) {
		auto e = domain.newEntity();
	}

	// 10 entites, but marked for deletion
	std::array<ecs::Entity, 10> toRecycle;
	for (size_t i = 0; i != 10; ++i) {
		toRecycle[i] = domain.newEntity();
	}

	// 10 entites
	for (size_t i = 0; i != 10; ++i) {
		auto e = domain.newEntity();
	}

	// 30 entities total, none deleted

	for (auto&& e : toRecycle) {
		// recycling existing entites/ids
		auto recycled1 = domain.recycleEntity(e);
		auto recycled2 = domain.recycleId(Traits::Id::part(e));
		// returns null entity
		EXPECT_EQ(recycled1, domain.null);
		EXPECT_EQ(recycled2, domain.null);
	}
}
