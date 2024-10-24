#include <tuple>

#include <Ecs.h>
#include <gtest/gtest.h>

namespace ecs = arch::ecs;

TEST(ECS, ComponentSimple) {
	// to create your own component you need to perform very specific steps:
	// 1. just create them
	// 2. Lorem ipsum
	// 3. You: Wait, it's that easy?
	// 4. Me:  Why would it be any different?

	struct Pos {
		float x;
		float y;
	};

	struct Vel {
		float x;
		float y;
	};

	// the above components are the simplest that can be
	// plain structs like those model many crucial concepts (on them later)

	ecs::Domain domain;

	// components can be then added to entites:
	auto e0 = domain.newEntity();

	// create component Pos for e0
	domain.addComponent<Pos>(e0);

	// they also are accessible by e0 handle:
	domain.getComponent<Pos>(e0).x = 4;
	domain.getComponent<Pos>(e0).y = 5;

	// each call to getComponent<T> is O(1):
	//		1 hash-map find, where hash result is compile-time
	//		2 vector find, by index
	// in Unity for example, analogous GetComponent<T> is O(n):
	//		going through list of components until T is found

	// each entity can only have 1 component of each type
	// this limits search time

	// attempting to create another component returns one already existing:
	{
		auto& oldPos = domain.getComponent<Pos>(e0);
		EXPECT_EQ(&oldPos, &domain.addComponent<Pos>(e0));
	}

	// let's add another entity
	auto e1 = domain.newEntity();

	// and it's components
	{
		// addComponent returns reference to created component
		auto& e1Pos = domain.addComponent<Pos>(e1, 1, 2);
		// arguments for creating Pos -------------^--^

		// addComponent without template parameters
		auto& e1Vel = domain.addComponent(e1, Vel{ .x = 4, .y = 6 });
	}

	// you can check if entity has component of given type
	EXPECT_TRUE(domain.hasComponent<Pos>(e0));
	EXPECT_FALSE(domain.hasComponent<Vel>(e0));
	EXPECT_TRUE(domain.hasComponent<Pos>(e1));
	EXPECT_TRUE(domain.hasComponent<Vel>(e1));

	// you can also remove components
	domain.removeComponent<Pos>(e1);

	// what happens when you call getComponent<T>, but entity does not contain it?
	// C++ standard calls this: Undefined Behavior (UB)
	// basicly whatever can happen (most likely SEGFAULT crash)

	// how can you then shield yourself from UB?
	// ofc by using optionals with tryGetComponent<T>
	{
		auto e1Pos = domain.tryGetComponent<Pos>(e1);

		// you can check if optional contains value:
		EXPECT_FALSE(e1Pos);
		EXPECT_FALSE(e1Pos.has_value());

		// if it contains something, access it:
		if (e1Pos or e1Pos.has_value()) {
			auto& e1PosReference = e1Pos->get();
			// ...
		}
	}

	// ECS counts how many components of specific type are there
	EXPECT_EQ(domain.count<Pos>(), 1);
	EXPECT_EQ(domain.count<Vel>(), 1);

	// you can also iterate over all components of any type
	for (auto&& [entity, pos] : domain.components<Pos>()) {
		// ...
	}
	for (auto&& [entity, vel] : domain.components<Vel>()) {
		// ...
	}
}

// what if you wanted to flag some entities?
// creating special component for that purpose is a solution

struct WorseEnemyFlag {};

// however component like this will occupy space (not efficient)
// better way is to explicitly mark this component as a flag

struct EnemyFlag {
	static constexpr bool flagComponent = true;
};

// if empty class (no non-static field) has static compile-time constant
// flagComponent/FlagComponent/flag_component = true, then it is considered a flag-component
// flag-components are never instantiated, therefore occupy less space

TEST(ECS, ComponentFlag) {
	ecs::Domain domain;

	auto e0 = domain.newEntity();
	auto e1 = domain.newEntity();
	auto e2 = domain.newEntity();
	auto e3 = domain.newEntity();

	domain.addComponent<EnemyFlag>(e2);
	domain.addComponent<EnemyFlag>(e3);

	// for flag-components, getComponent<T> == hasComponent<T>
	EXPECT_EQ(typeid(domain.getComponent<EnemyFlag>(e2)), typeid(domain.hasComponent<EnemyFlag>(e2)));

	// instead of reference to component, getComponent<T> returns bool
	EXPECT_TRUE(domain.getComponent<EnemyFlag>(e2));
	EXPECT_TRUE(domain.getComponent<EnemyFlag>(e3));
	// meaning getComponent<T> will never result in an UB
	EXPECT_FALSE(domain.getComponent<EnemyFlag>(e0));
	EXPECT_FALSE(domain.getComponent<EnemyFlag>(e1));

	for (auto&& [entity, vel] : domain.components<EnemyFlag>()) {
		// ...
	}
}

// now let's dive deep into implementation details

// in C++ there is a thing called 'concepts'
// they describe requirements for types, that need to be satisfied
// starting from memory size of type, ending at minute details about its methods

// if type 'T' satisfies all requirements of a concept 'C', then we say that T models C
// for example float and double model std::floating_point and lambdas model std::invocable

// concepts refine older C++ concept: Named Requirements
// those are abstract rules that types must follow if thew want to perform specific operaions
// concepts were added in C++20, meaning that for 22 years C++ standard library was written without them
// huge amount of std code does not depend on them, being the reason why C++ errors are so bizzare-looking
// (not to mention that for ~12 years C++ was not standardized)

// the most important concept used in this ECS is std::movable
// it requires types to:
// be move-constructible (have move-constructor)
// be move-assignable (have move-assign operator)
// be swappable (have method swap() or have specialized std::swap)

// now that you know what concepts are and understand what std::movable types are capable of
// I can now explain what in-place components are

// in-place components are components which are not moved in the internal storage on basic operations like add/remove
// this means that any pointers or references to them will not be invalidated while performing
// operationson the other components of that type, in contrast to regular components
// this also means that storage for in-place components might not be random-access
// marking components are in-place components works simmilar to flag components
// you need to make compile-time constant named inPlaceComponent/InPlaceComponent/in_place_component = true

struct Ship {
	static constexpr bool inPlaceComponent = true;

	float health;
	float bulletDamage;
};

// by default components are not in-place components
// you can still explicitly mark them as not in-place
// however if you do that, while components does not model std::movable
// you will get a compilation error, because non-movable components can only be in-place
// and non-movables will be marked as in-place by default

// I have mentioned 'internal storage' before, but what does it mean?
// every instance of 'Ship', requires 8 bytes of memory
// this memory is handled internally by ComponentPools
// by default, this memory for components is segmented into pages of size 1024 (instances, not bytes)
// thanks to this, add operation on components will not reallocate nor move any components in memory
// It may only allocate new page if previous pages are full
// just as all the other settings, this also can be customized in class by compile-time constant
// componentPageSize/ComponentPageSize/component_page_size = <page-size>
// pageSize is ignored if component is marked as flag-component
// ATTENTION! page size must be a power of two, to aquire (2^N) just write (1 << N)

// those 3 settings can be written in definition of your class
// but it can also be put into specialization of arch::ecs::ComponentSpecs<T>, which will override all other settings
// contrary to in-class settings, this method requires you to explicitly set all settings, not just one

template<>
struct arch::ecs::ComponentSpecs<Ship> {
	// prolonging in-class setting
	// if set to false, in-class setting would be overridden
	static constexpr bool inPlace = true;

	// not empty -> not a flag
	static constexpr bool flag = false;

	// let's change pageSize to something different
	// for example: 2^5 = 32
	static constexpr size_t pageSize = (1 << 5);
};

// let's now check if our settings acually work

TEST(ECS, ComponentInPlaceCustomPageSize) {
	ecs::Domain domain;

	std::array<ecs::Entity, 32 * 3> entities;
	for (size_t i = 0; i != 32 * 3; ++i) {
		entities[i] = domain.newEntity();

		domain.addComponent<Ship>(entities[i]);
	}

	// for later
	auto& shipOf0 = domain.getComponent<Ship>(entities[0]);
	auto& shipOf31 = domain.getComponent<Ship>(entities[31]);

	for (size_t i = 0; i != 32; ++i) {
		// getting ships 32 (pageSize) instances apart
		auto& ship1 = domain.getComponent<Ship>(entities[i]);
		auto& ship2 = domain.getComponent<Ship>(entities[i + 32]);
		auto& ship3 = domain.getComponent<Ship>(entities[i + 64]);

		// normally ship2 would be 32 instances furhter than ship2
		// but since they are on different pages, they are not
		// (there still is non-zero chance of them being that close, because of that tests below are commented out)
		// ASSERT_NE(&ship1 + 32, &ship2);
		// ASSERT_NE(&ship1 + 64, &ship3);
		// ASSERT_NE(&ship2 + 32, &ship3);
	}

	for (size_t i = 1; i != 31; ++i) {
		// entities 0-31 have Ship component
		// let's remove them from 1-30
		domain.removeComponent<Ship>(entities[i]);
	}

	// now we can be certain that Ships for 0 and 31 are not close in memory
	// (by close means one instance after another)
	ASSERT_NE(abs(&domain.getComponent<Ship>(entities[31]) - &domain.getComponent<Ship>(entities[0])), sizeof(Ship));

	// also their place in memory has not changed
	ASSERT_EQ(&shipOf0, &domain.getComponent<Ship>(entities[0]));
	ASSERT_EQ(&shipOf31, &domain.getComponent<Ship>(entities[31]));
}

// this is mostly it for components
// remember that these are just examples
// not every components needs to be full-public struct
// ECS supports classes with private fields/methods
// but doesn't suppot polymorphic classes
// because being polymorphic makes memory of an object
// very complex with virtual tables, and very 'implementation-defined'
