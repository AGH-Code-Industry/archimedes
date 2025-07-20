#include <algorithm>
#include <random>
#include <ranges>
#include <unordered_set>

#include <Ecs.h>
#include <gtest/gtest.h>

using namespace arch;
using ETraits = ecs::_details::EntityTraits;

TEST(ECS, Entity_Create) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
	}

	// are all entities unique?
	auto entities = domain.entities() | std::ranges::to<std::unordered_set>();
	ASSERT_EQ(entities.size(), entityCount);

	// are all entities alive?
	ASSERT_TRUE(std::ranges::all_of(domain.entities(), [&](auto entity) { return domain.alive(entity); }));
}

TEST(ECS, Entity_Valid) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	for (int i = 0; i != entityCount; ++i) {
		domain.newEntity();
	}

	// kill random half of entities
	auto entities = domain.entities() | std::ranges::to<std::vector>();
	auto rng = std::mt19937(std::random_device{}());
	std::ranges::shuffle(entities, rng);
	auto toRemove = entities | std::views::take(entityCount / 2);
	domain.kill(toRemove.begin(), toRemove.end());

	// are half of entities alive?
	ASSERT_EQ(domain.count(), toRemove.size());

	// are removed entities alive?
	ASSERT_TRUE(std::ranges::all_of(toRemove, [&](auto entity) { return !domain.alive(entity); }));

	// are non-removed entities alive?
	ASSERT_TRUE(std::ranges::all_of(entities | std::views::drop(toRemove.size()), [&](auto entity) {
		return domain.alive(entity);
	}));

	// are removed IDs alive after recycling
	for (int i = 0; i != toRemove.size(); ++i) {
		domain.newEntity();
	}
	ASSERT_TRUE(std::ranges::all_of(toRemove, [&](auto entity) { return !domain.alive(entity); }));

	// are some random entities alive?
	auto distrib = std::uniform_int_distribution<u64>(entityCount, entityCount * 10);
	for (int i = 0; i != entityCount; ++i) {
		ASSERT_FALSE(domain.alive(ETraits::Ent::fromParts(distrib(rng), 0)));
	}
}

TEST(ECS, Entity_Recycle) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	for (int i = 0; i != entityCount; ++i) {
		domain.newEntity();
	}

	auto entities = domain.entities() | std::ranges::to<std::vector>();

	domain.kill(entities.begin(), entities.end());

	// are entites queued in order they were killed?
	for (int i = 0; i != entityCount; ++i) {
		ASSERT_EQ(ETraits::Id::part(domain.newEntity()), ETraits::Id::part(entities.rbegin()[i]));
	}

	// are old entities dead?
	ASSERT_TRUE(std::ranges::all_of(entities, [&](auto entity) { return !domain.alive(entity); }));

	// were old entities recycled?
	ASSERT_TRUE(std::ranges::all_of(entities, [&](auto entity) { return domain.contains(ETraits::Id::part(entity)); }));

	// were versions incremented?
	ASSERT_TRUE(std::ranges::all_of(entities, [&](auto entity) {
		return domain.version(ETraits::Id::part(entity)) == 1;
	}));
}

class Component {
public:
	// increments on construct
	Component(int* value) noexcept: _value{ value } { ++(*_value); }

	// decrements on destruct
	~Component() noexcept {
		--(*_value);
		_value = nullptr;
	}

private:
	int* _value;
};

class InPlaceComponent {
public:
	static constexpr bool inPlaceComponent = true;

	// increments on construct
	InPlaceComponent(int* value) noexcept: _value{ value } { ++(*_value); }

	// decrements on destruct
	~InPlaceComponent() noexcept {
		--(*_value);
		_value = nullptr;
	}

private:
	int* _value;
};

struct FlagComponent {
	static constexpr bool flagComponent = true;
};

TEST(ECS, Entity_KillDestroysComponents) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	auto rng = std::mt19937(std::random_device{}());
	auto dist = std::uniform_int_distribution(0, 2);

	int componentCount = 0;
	int inPlaceComponentCount = 0;

	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		switch (dist(rng)) {
			case 0: domain.addComponent<Component>(entity, &componentCount); break;
			case 1: domain.addComponent<InPlaceComponent>(entity, &inPlaceComponentCount); break;
			case 2: domain.addComponent<FlagComponent>(entity); break;

			// do all entities have components?
			default: FAIL();
		}
	}

	// counters for manual decrement
	int componentCount2 = componentCount;
	int inPlaceComponentCount2 = inPlaceComponentCount;
	int flagComponentCount2 = domain.count<FlagComponent>();

	for (auto entity : domain.entities() | std::ranges::to<std::vector>()) {
		// manually decrement values
		if (domain.hasComponent<Component>(entity)) {
			--componentCount2;
		} else if (domain.hasComponent<InPlaceComponent>(entity)) {
			--inPlaceComponentCount2;
		} else if (domain.hasComponent<FlagComponent>(entity)) {
			--flagComponentCount2;
		}

		domain.kill(entity);

		// was component correctly destructed? (counters' values match)
		ASSERT_EQ(componentCount, componentCount2);
		ASSERT_EQ(inPlaceComponentCount, inPlaceComponentCount2);
		ASSERT_EQ(domain.count<FlagComponent>(), flagComponentCount2);
	}
}

TEST(ECS, Entity_EmptyPool) {
	ecs::Domain domain;

	// no entities -> begin == end
	ASSERT_EQ(domain.entities().begin(), domain.entities().end());
}
