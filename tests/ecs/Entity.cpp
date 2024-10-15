#include <Ecs.h>
#include <gtest/gtest.h>

namespace ecs = arch::ecs;
using Traits = ecs::_details::EntityTraits<ecs::e64>;

TEST(ECS, EntityCreationAlive) {
	// e64 - 64-bit entity handle
	// there also exists e32, with smaller range
	ecs::Domain<ecs::e64> domain;

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
	ecs::Domain<ecs::e64> domain;
	std::array<ecs::e64, 10> toDelete;

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
	ecs::Domain<ecs::e64> domain;

	// 10 entites
	for (size_t i = 0; i != 10; ++i) {
		auto e = domain.newEntity();
	}

	// 10 entites, but marked for deletion
	std::array<ecs::e64, 10> toDelete;
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
	ecs::Domain<ecs::e64> domain;

	// 10 entites
	for (size_t i = 0; i != 10; ++i) {
		auto e = domain.newEntity();
	}

	// 10 entites, but marked for deletion
	std::array<ecs::e64, 10> toRecycle;
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

// normally there are two entity types: e32 and e64
// they are not special in themselves, just enum types
// ECS allows you to create your own entity types like this:

// 16-bit entity
enum class Entity16Bit : uint16_t;

// however, Entity16Bit is not usable yet
// lets create example configuration
struct Entity16BitConfiguration {
	// type of entity
	using EntityT = Entity16Bit;
	// bit lenght of id part of handle, for example 12
	static inline constexpr size_t idLength = 12;
	// type of id part of handle, must be big enough to store 12 bits
	using IdT = uint16_t;
	// type of version part of handle, must be big enough to store 4 bits
	using VersionT = uint8_t;
	// size of pages in sparse sets, default is 1024
	// ATTENTION! pageSize must be a power of 2
	// tip: (2^N) can be written as (1 << N)
	static inline constexpr size_t pageSize = 1'024;
};

// to be visible to ECS you need to specialize arch::ecs::EntitySpecs<Entity16Bit>
// you can use AutoEntitySpecs<EntityT, VersionT, idLength, pageSize>
template<>
struct arch::ecs::EntitySpecs<Entity16Bit>: arch::ecs::AutoEntitySpecs<Entity16Bit, uint8_t, 12, 1'024> {};

//			or your previous configuration: Entity16BitConfiguration

// and thats basically it, ECS will compute rest of the info by itself
// you can now proudly use you entities

TEST(ECS, EntityCustom) {
	ecs::Domain<Entity16Bit> domain;

	auto e0 = domain.newEntity();

	// ...

	EXPECT_EQ(typeid(e0), typeid(Entity16Bit));
}
