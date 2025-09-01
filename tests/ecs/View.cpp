#include <tuple>

#include <archimedes/Ecs.h>
#include <gtest/gtest.h>

namespace ecs = arch::ecs;

TEST(ECS, ViewsSimple) {
	struct Pos {
		float x;
		float y;
	};

	struct Vel {
		float x;
		float y;
	};

	ecs::Domain domain;

	// let's create some entities with some components

	for (size_t i = 0; i != 10'000; ++i) {
		auto e = domain.newEntity();

		domain.addComponent<Pos>(e);
		auto ifToAdd = rand();
		if (i % (ifToAdd ? ifToAdd : 1)) {
			domain.addComponent<Vel>(e);
		}
	}

	// that sure is a lot of entites and components
	// now let's update their positions
	//
	// ...
	//
	// wait, which ones?
	// if only you could find all entites with Pos and Vel
	// actually, you can using views

	{ auto viewPosVal = domain.view<Pos, Vel>(); }

	// hold on, what if you accidentaly also update Vel?
	// add 'const' to Vel to make it readonly, any change to Vel will now be an error

	auto viewPosVel = domain.view<Pos, const Vel>();

	// now you have to choose how you update all positions

	// option 1. manually obtain components
	for (ecs::Entity entity : viewPosVel) {
		{
			// get tuple with references
			auto posVelTuple = viewPosVel.get(entity);
			// access tuple
			auto& pos = std::get<0>(posVelTuple);
			auto& vel = std::get<1>(posVelTuple);
		}

		// the above can be abbreviated with:
		auto&& [pos, vel] = viewPosVel.get(entity);

		pos.x += vel.x;
		pos.y += vel.y;

		break; // exit for
	}

	// option 2. use view.all()
	for (auto&& [entity, pos, vel] : viewPosVel.all()) {
		pos.x += vel.x;
		pos.y += vel.y;

		break; // exit for
	}

	// option 3. use view.forEach(entity)
	viewPosVel.forEach([&viewPosVel /* capture view */](ecs::Entity entity) {
		// auto&& [pos, vel] = viewPosVel.get(entity);

		// pos.x += vel.x;
		// pos.y += vel.y;
	});

	// option 4. use view.forEach(entity, &pos, const &vel)
	viewPosVel.forEach([&viewPosVel /* capture view */](ecs::Entity entity, Pos& pos, const Vel& vel) {
		// pos.x += vel.x;
		// pos.y += vel.y;
	});
}
