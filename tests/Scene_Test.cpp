#include <array>

#include <Scene.h>
#include <gtest/gtest.h>

TEST(Scene, HierarchyIteration) {
	arch::Scene scene;

	auto e = std::array<arch::ecs::Entity, 11>();

	for (auto&& _e : e) {
		_e = scene.newEntity();
	}

	scene.setParent(e[1], e[0]);
	scene.setParent(e[2], e[0]);
	scene.setParent(e[3], e[0]);
	{
		scene.setParent(e[4], e[3]);
		{ scene.setParent(e[5], e[4]); }
		scene.setParent(e[6], e[3]);
	}
	scene.setParent(e[7], e[0]);
	{ scene.setParent(e[8], e[7]); }
	scene.setParent(e[9], e[0]);

	ASSERT_TRUE(std::ranges::equal(e, scene.rootNode().recursiveIterable()));
	ASSERT_TRUE(std::ranges::equal(std::views::reverse(e), std::views::reverse(scene.rootNode().recursiveIterable())));
}
