#include <algorithm>
#include <array>
#include <print>
#include <random>
#include <ranges>
#include <unordered_set>
#include <vector>

#include <archimedes/Ecs.h>
#include <archimedes/utils/CallableTraits.h>
#include <archimedes/utils/IsApplicable.h>
#include <gtest/gtest.h>

using namespace arch;

namespace {

struct NormalComponent {
	int value = 0;
};

struct NormalComponent2 {
	int value = 0;
};

struct InPlaceComponent: NormalComponent, ecs::InPlaceComponent {};

struct FlagComponent: ecs::FlagComponent {};

} // namespace

TEST(ECS, View_OneComponent) {
	ecs::Domain domain;

	constexpr int entityCount = ecs::ComponentSpecs<NormalComponent>::pageSize;

	// every entity has NormalComponent1
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<NormalComponent>(entity);
		domain.addComponent<NormalComponent2>(entity);
	}

	// do all entities have component?
	ASSERT_TRUE(std::ranges::equal(domain.entities(), domain.view<NormalComponent>()));

	// pairwise view of components
	auto v = domain.view<NormalComponent, NormalComponent2>().comps();
	auto componentPairs = v | std::views::transform([](auto tuple) -> auto& {
							  return std::get<0>(tuple);
						  }) |
		std::views::pairwise;
	ASSERT_TRUE(std::ranges::all_of(componentPairs, [](auto pair) {
		auto&& [first, second] = pair;
		// are all components on a page adjacent?
		return &second == &first + 1;
	}));

	// increment value of each component
	domain.view<NormalComponent>().forEach([](auto& normal) {
		++normal.value;
	});

	// sum values
	int sum = 0;
	for (auto&& [normal] : domain.view<NormalComponent>().comps()) {
		sum += normal.value;
	}

	// is sum == entityCount?
	ASSERT_EQ(sum, entityCount);
}

TEST(ECS, View_OneContinuousComponentIsRandomAccess) {
	ecs::Domain domain;

	std::array entities{
		domain.newEntity(),
		domain.newEntity(),
		domain.newEntity(),
		domain.newEntity(),
	};
	for (int i = 0; i != static_cast<int>(entities.size()); ++i) {
		domain.addComponent<NormalComponent>(entities[i]).value = i;
	}

	auto view = domain.view<NormalComponent>();
	static_assert(std::ranges::random_access_range<decltype(view)>);

	ASSERT_EQ(std::ranges::distance(view), entities.size());
	ASSERT_EQ(view.begin()[2], entities[2]);
	ASSERT_EQ(*(2 + view.begin()), entities[2]);

	// Removing a non-in-place component fills the gap with the last component.
	domain.removeComponent<NormalComponent>(entities[1]);
	auto viewAfterRemoval = domain.view<NormalComponent>();
	const std::array expectedEntities{ entities[0], entities[3], entities[2] };
	const std::array expectedValues{ 0, 3, 2 };

	EXPECT_EQ(std::ranges::distance(viewAfterRemoval), expectedEntities.size());
	EXPECT_TRUE(std::ranges::equal(viewAfterRemoval, expectedEntities));
	auto values = viewAfterRemoval.comps() | std::views::transform([](auto componentTuple) {
		return std::get<0>(componentTuple).value;
	});
	EXPECT_TRUE(std::ranges::equal(values, expectedValues));
}

TEST(ECS, View_OneInPlaceComponentSkipsHoles) {
	ecs::Domain domain;

	std::array entities{
		domain.newEntity(),
		domain.newEntity(),
		domain.newEntity(),
		domain.newEntity(),
	};
	for (int i = 0; i != static_cast<int>(entities.size()); ++i) {
		domain.addComponent<InPlaceComponent>(entities[i]).value = i;
	}

	domain.removeComponent<InPlaceComponent>(entities[1]);
	auto view = domain.view<InPlaceComponent>();
	static_assert(std::ranges::bidirectional_range<decltype(view)>);
	static_assert(!std::ranges::random_access_range<decltype(view)>);

	const std::array expectedEntities{ entities[0], entities[2], entities[3] };
	const std::array expectedReversed{ entities[3], entities[2], entities[0] };
	EXPECT_TRUE(std::ranges::equal(view, expectedEntities));
	EXPECT_TRUE(std::ranges::equal(view | std::views::reverse, expectedReversed));
}

TEST(ECS, View_OneIncludeAndOneExcludeFiltersEveryAccessPath) {
	ecs::Domain domain;

	std::array<ecs::Entity, 7> entities;
	for (int i = 0; i != static_cast<int>(entities.size()); ++i) {
		entities[i] = domain.newEntity();
		domain.addComponent<NormalComponent>(entities[i]).value = i;
	}
	for (const auto index : { 0, 3, 4, 6 }) {
		domain.addComponent<InPlaceComponent>(entities[index]);
	}

	auto view = domain.view<NormalComponent>(exclude<InPlaceComponent>);
	static_assert(std::ranges::bidirectional_range<decltype(view)>);
	static_assert(!std::ranges::random_access_range<decltype(view)>);

	const std::array expectedEntities{ entities[1], entities[2], entities[5] };
	const std::array expectedReversed{ entities[5], entities[2], entities[1] };
	const std::array expectedValues{ 1, 2, 5 };

	ASSERT_TRUE(std::ranges::equal(view, expectedEntities));
	ASSERT_TRUE(std::ranges::equal(view | std::views::reverse, expectedReversed));

	std::vector<ecs::Entity> forEachEntities;
	std::vector<int> forEachValues;
	view.forEach([&](const ecs::Entity entity, NormalComponent& component) {
		forEachEntities.push_back(entity);
		forEachValues.push_back(component.value);
	});
	EXPECT_TRUE(std::ranges::equal(forEachEntities, expectedEntities));
	EXPECT_TRUE(std::ranges::equal(forEachValues, expectedValues));

	auto componentValues = view.comps() | std::views::transform([](auto componentTuple) {
		return std::get<0>(componentTuple).value;
	});
	EXPECT_TRUE(std::ranges::equal(componentValues, expectedValues));

	auto entityComponents = view.entityComps();
	auto expectedEntity = expectedEntities.begin();
	auto expectedValue = expectedValues.begin();
	for (auto&& [entity, component] : entityComponents) {
		ASSERT_NE(expectedEntity, expectedEntities.end());
		EXPECT_EQ(entity, *expectedEntity++);
		EXPECT_EQ(component.value, *expectedValue++);
	}
	ASSERT_EQ(expectedEntity, expectedEntities.end());
	ASSERT_EQ(expectedValue, expectedValues.end());

	for (const auto entity : expectedEntities) {
		ASSERT_TRUE(view.contains(entity));
	}
	for (const auto index : { 0, 3, 4, 6 }) {
		ASSERT_FALSE(view.contains(entities[index]));
	}
}

TEST(ECS, View_OneIncludeIgnoresMissingExcludePool) {
	ecs::Domain domain;

	std::array<ecs::Entity, 3> entities;
	for (auto& entity : entities) {
		entity = domain.newEntity();
		domain.addComponent<NormalComponent>(entity);
	}

	ASSERT_TRUE(std::ranges::equal(domain.view<NormalComponent>(exclude<int>), entities));
}

TEST(ECS, View_TwoComponents) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	// every entity has NormalComponent1, half have NormalComponent2
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<NormalComponent>(entity);
		if (i % 2) {
			domain.addComponent<InPlaceComponent>(entity);
		}
	}

	// do views overlap as expected?
	ASSERT_TRUE(std::ranges::equal(domain.view<InPlaceComponent>(), domain.view<NormalComponent, InPlaceComponent>()));
}

TEST(ECS, View_Empty) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	// every entity has NormalComponent1, half have NormalComponent2, none have FlagComponent
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<NormalComponent>(entity);
		if (i % 2) {
			domain.addComponent<InPlaceComponent>(entity);
		}
	}

	// does View correctly selected minCPool?
	ASSERT_EQ((domain.view<NormalComponent, InPlaceComponent>().minCPool()), 1);

	// does ComponentPool<FlagComponent> exist?
	ASSERT_EQ((domain.view<NormalComponent, InPlaceComponent, FlagComponent>().minCPool()), (u32)-1);

	// is view with FlagComponent empty?
	ASSERT_TRUE(std::ranges::equal(
		std::views::empty<ecs::Entity>,
		domain.view<NormalComponent, InPlaceComponent, FlagComponent>()
	));
	ASSERT_TRUE(std::ranges::equal(std::views::empty<ecs::Entity>, domain.view<FlagComponent>()));
}

TEST(ECS, View_EmptyWithExclude) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	// every entity has all the components
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<NormalComponent>(entity);
		domain.addComponent<InPlaceComponent>(entity);
		domain.addComponent<FlagComponent>(entity);
	}

	// are the below views emtpy as they should be?
	ASSERT_TRUE(std::ranges::empty(domain.view<NormalComponent>(exclude<InPlaceComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<NormalComponent>(exclude<FlagComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<InPlaceComponent>(exclude<NormalComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<InPlaceComponent>(exclude<FlagComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<FlagComponent>(exclude<NormalComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<FlagComponent>(exclude<InPlaceComponent>)));

	// Below would not compile, as overlapping views do not make sense
	// ASSERT_TRUE(std::ranges::empty(domain.view<NormalComponent>(exclude<NormalComponent>)));
	// ASSERT_TRUE(std::ranges::empty(domain.view<InPlaceComponent>(exclude<InPlaceComponent>)));
	// ASSERT_TRUE(std::ranges::empty(domain.view<FlagComponent>(exclude<FlagComponent>)));
}

TEST(ECS, View_AfterRemoval) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	// all entities have all the components
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<NormalComponent>(entity);
		domain.addComponent<InPlaceComponent>(entity);
		domain.addComponent<FlagComponent>(entity);
	}

	auto rng = std::mt19937(std::random_device{}());
	auto dist = std::uniform_int_distribution(0, 2);

	// about half the entities have one component removed
	for (auto entity : domain.entities()) {
		if (rng() % 2) {
			switch (dist(rng)) {
				case 0: domain.removeComponent<NormalComponent>(entity); break;
				case 1: domain.removeComponent<InPlaceComponent>(entity); break;
				case 2: domain.removeComponent<FlagComponent>(entity); break;
			}
		}
	}

	auto expectedEntities = domain.entities() | std::views::filter([&domain](auto entity) {
								return domain.hasComponent<NormalComponent>(entity) &&
									domain.hasComponent<InPlaceComponent>(entity) &&
									domain.hasComponent<FlagComponent>(entity);
							}) |
		std::ranges::to<std::unordered_set>();

	auto actualEntities = domain.view<NormalComponent, InPlaceComponent, FlagComponent>() |
		std::ranges::to<std::unordered_set>();

	// are entities from view the extected ones?
	ASSERT_EQ(actualEntities, expectedEntities);
}

TEST(ECS, View_WithExcludes) {
	ecs::Domain domain;

	constexpr int entityCount = 1'000;

	// all entities have all the components
	for (int i = 0; i != entityCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<NormalComponent>(entity);
		domain.addComponent<InPlaceComponent>(entity);
		domain.addComponent<FlagComponent>(entity);
	}

	auto rng = std::mt19937(std::random_device{}());

	// about half the entities have int
	for (auto entity : domain.entities()) {
		if (rng() % 2) {
			domain.addComponent<int>(entity);
		}
	}

	// has all the components, and has no int
	auto expectedEntities = domain.entities() | std::views::filter([&domain](auto entity) {
								return domain.hasComponent<NormalComponent>(entity) &&
									domain.hasComponent<InPlaceComponent>(entity) &&
									domain.hasComponent<FlagComponent>(entity) && !domain.hasComponent<int>(entity);
							}) |
		std::ranges::to<std::unordered_set>();

	auto actualEntities = domain.view<NormalComponent, InPlaceComponent, FlagComponent>(exclude<int>) |
		std::ranges::to<std::unordered_set>();

	// are entities from view the extected ones?
	ASSERT_EQ(actualEntities, expectedEntities);
}

TEST(ECS, View_OnlyExcludes) {
	ecs::Domain domain;

	constexpr int excludedCount = 1'000;
	constexpr int notExcludedCount = 1'000;

	std::vector<ecs::Entity> notExcludedEntities;
	notExcludedEntities.resize(notExcludedCount);

	for (int i = 0; i != excludedCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<NormalComponent>(entity);
		domain.addComponent<NormalComponent2>(entity);
		domain.addComponent<FlagComponent>(entity);
		domain.addComponent<InPlaceComponent>(entity);
	}

	for (int i = 0 ; i != notExcludedCount; ++i) {
		auto entity = domain.newEntity();
		domain.addComponent<NormalComponent2>(entity);
		notExcludedEntities[i] = entity;
	}

	ASSERT_TRUE(std::ranges::equal(domain.view<>(exclude<NormalComponent>), notExcludedEntities));
	ASSERT_TRUE(std::ranges::equal(domain.view<>(exclude<FlagComponent>), notExcludedEntities));
	ASSERT_TRUE(std::ranges::equal(domain.view<>(exclude<InPlaceComponent>), notExcludedEntities));
}
