#include <print>
#include <random>
#include <ranges>
#include <unordered_set>

#include <Ecs.h>
#include <gtest/gtest.h>
#include <tUtils/Functions/CallableTraits.h>
#include <tUtils/Functions/IsApplicable.h>

using namespace arch;

struct NormalComponent {
	int value = 0;
};

struct InPlaceComponent: NormalComponent {
	static constexpr bool inPlaceComponent = true;
};

struct FlagComponent {
	static constexpr bool flagComponent = true;
};

TEST(ECS, View_OneComponent) {
	ecs::Domain domain;

	constexpr int entityCount = ecs::ComponentSpecs<NormalComponent>::pageSize;

	// every entity has NormalComponent1
	for (int i = 0; i != entityCount; ++i) {
		domain.addComponent<NormalComponent>(domain.newEntity());
	}

	// do all entities have component?
	ASSERT_TRUE(std::ranges::equal(domain.entities(), domain.view<NormalComponent>()));

	// pairwise view of components
	auto componentPairs = domain.view<NormalComponent>().components()
		| std::views::transform([](auto tuple) -> auto& { return std::get<0>(tuple); })
		| std::views::pairwise;
	ASSERT_TRUE(std::ranges::all_of(componentPairs, [](auto pair) {
		auto&& [first, second] = pair;
		// are all components on a page adjacent?
		return &second == &first + 1;
	}));

	// increment value of each component
	domain.view<NormalComponent>().forEach([](auto& normal) { ++normal.value; });

	// sum values
	int sum = 0;
	for (auto&& [normal] : domain.view<NormalComponent>().components()) {
		sum += normal.value;
	}

	// is sum == entityCount?
	ASSERT_EQ(sum, entityCount);
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
	ASSERT_TRUE(
		std::ranges::equal(
			std::views::empty<ecs::Entity>,
			domain.view<NormalComponent, InPlaceComponent, FlagComponent>()
		)
	);
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
	ASSERT_TRUE(std::ranges::empty(domain.view<NormalComponent>(exclude<NormalComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<NormalComponent>(exclude<InPlaceComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<NormalComponent>(exclude<FlagComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<InPlaceComponent>(exclude<NormalComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<InPlaceComponent>(exclude<InPlaceComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<InPlaceComponent>(exclude<FlagComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<FlagComponent>(exclude<NormalComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<FlagComponent>(exclude<InPlaceComponent>)));
	ASSERT_TRUE(std::ranges::empty(domain.view<FlagComponent>(exclude<FlagComponent>)));
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

	auto expectedEntities = domain.entities()
		| std::views::filter([&domain](auto entity) {
								return domain.hasComponent<NormalComponent>(entity)
									&& domain.hasComponent<InPlaceComponent>(entity)
									&& domain.hasComponent<FlagComponent>(entity);
							})
		| std::ranges::to<std::unordered_set>();

	auto actualEntities = domain.view<NormalComponent, InPlaceComponent, FlagComponent>()
		| std::ranges::to<std::unordered_set>();

	// are entities from view the extected ones?
	ASSERT_EQ(actualEntities, expectedEntities);
}
