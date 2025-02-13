#include <array>

#include <Scene.h>
#include <gtest/gtest.h>

TEST(Scene, HierarchyIteration) {
	arch::scene::Scene scene;

	auto e = std::array<arch::hier::HierarchyNode*, 11>();

	for (size_t i = 0; i != 11; ++i) {
		e[i] = &scene.domain().getComponent<arch::hier::HierarchyNode>(scene.newEntity());
	}

	e[1]->setParent(*e[0]);
	e[2]->setParent(*e[0]);
	e[3]->setParent(*e[0]);

	{
		e[4]->setParent(*e[3]);
		{ e[5]->setParent(*e[4]); }
		e[6]->setParent(*e[3]);
	}
	e[7]->setParent(*e[0]);
	{ e[8]->setParent(*e[7]); }
	e[9]->setParent(*e[0]);

	ASSERT_TRUE(std::ranges::equal(
		e,
		scene.root().getComponent<arch::hier::HierarchyNode>().recursiveIterable(),
		{},
		[](const arch::hier::HierarchyNode* node) { return node->entity(); }
	));
	ASSERT_TRUE(std::ranges::equal(
		std::views::reverse(e),
		std::views::reverse(scene.root().getComponent<arch::hier::HierarchyNode>().recursiveIterable()),
		{},
		[](const arch::hier::HierarchyNode* node) { return node->entity(); }
	));
}
