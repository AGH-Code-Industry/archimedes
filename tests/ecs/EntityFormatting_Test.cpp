#include <format>
#include <print>
#include <ranges>

#include <archimedes/Ecs.h>
#include <gtest/gtest.h>

using namespace arch;

// comments with numbers show expected format

TEST(ECS, Entity_Formatting_Basic) {
	ecs::Domain domain;

	for (u64 i = 0; i <= 100; ++i) {
		auto entity = domain.newEntity();

		// 240
		ASSERT_EQ(std::format("{}", entity), std::format("{}", i));
	}

	// null
	ASSERT_EQ(std::format("{}", ecs::nullEntity), "null");
}

TEST(ECS, Entity_Formatting_Bases) {
	ecs::Domain domain;

	for (u64 i = 0; i <= 100; ++i) {
		auto entity = domain.newEntity();

		// 11110000
		ASSERT_EQ(std::format("{:#b}", entity), std::format("{:#b}", i));
		// 11110000
		ASSERT_EQ(std::format("{:#B}", entity), std::format("{:#B}", i));
		// 360
		ASSERT_EQ(std::format("{:#o}", entity), std::format("{:#o}", i));
		// f0
		ASSERT_EQ(std::format("{:#x}", entity), std::format("{:#x}", i));
		// F0
		ASSERT_EQ(std::format("{:#X}", entity), std::format("{:#X}", i));

		// 0b11110000
		ASSERT_EQ(std::format("{:#b}", entity), std::format("{:#b}", i));
		// 0B11110000
		ASSERT_EQ(std::format("{:#B}", entity), std::format("{:#B}", i));
		// 0360
		ASSERT_EQ(std::format("{:#o}", entity), std::format("{:#o}", i));
		// 0xf0
		ASSERT_EQ(std::format("{:#x}", entity), std::format("{:#x}", i));
		// 0XF0
		ASSERT_EQ(std::format("{:#X}", entity), std::format("{:#X}", i));
	}
}

TEST(ECS, Entity_Formatting_FillAndAlign) {
	ecs::Domain domain;

	for (u64 i = 0; i <= 100; ++i) {
		auto entity = domain.newEntity();

		// 240
		ASSERT_EQ(std::format("{:*>}", entity), std::format("{:*>}", i));
		// 240
		ASSERT_EQ(std::format("{:>}", entity), std::format("{:>}", i));
		// 240
		ASSERT_EQ(std::format("{:*<}", entity), std::format("{:*<}", i));
		// 240
		ASSERT_EQ(std::format("{:*^}", entity), std::format("{:*^}", i));

		// *******240
		ASSERT_EQ(std::format("{:*>10}", entity), std::format("{:*>10}", i));
		// <7 spaces>240
		ASSERT_EQ(std::format("{:>10}", entity), std::format("{:>10}", i));
		// 240*******
		ASSERT_EQ(std::format("{:*<10}", entity), std::format("{:*<10}", i));
		// ***240****
		ASSERT_EQ(std::format("{:*^10}", entity), std::format("{:*^10}", i));

		// ********f0
		ASSERT_EQ(std::format("{:*>10x}", entity), std::format("{:*>10x}", i));
		// <8 spaces>f0
		ASSERT_EQ(std::format("{:>10x}", entity), std::format("{:>10x}", i));
		// f0********
		ASSERT_EQ(std::format("{:*<10x}", entity), std::format("{:*<10x}", i));
		// ****f0****
		ASSERT_EQ(std::format("{:*^10x}", entity), std::format("{:*^10x}", i));

		// *******240
		ASSERT_EQ(std::format("{:*>010}", entity), std::format("{:*>010}", i));
		// <7 spaces>240
		ASSERT_EQ(std::format("{:>010}", entity), std::format("{:>010}", i));
		// 240*******
		ASSERT_EQ(std::format("{:*<010}", entity), std::format("{:*<010}", i));
		// ***240****
		ASSERT_EQ(std::format("{:*^010}", entity), std::format("{:*^010}", i));
	}
}

TEST(ECS, Entity_Formatting_Width) {
	ecs::Domain domain;

	for (u64 i = 0; i <= 100; ++i) {
		auto entity = domain.newEntity();

		// *******240
		ASSERT_EQ(std::format("{:*>{}}", entity, 10), std::format("{:*>{}}", i, 10));
		// 240*******
		ASSERT_EQ(std::format("{:*<{}}", entity, 10), std::format("{:*<{}}", i, 10));
		// ***240****
		ASSERT_EQ(std::format("{:*^{}}", entity, 10), std::format("{:*^{}}", i, 10));

		// *******240
		ASSERT_EQ(std::format("{1:*>{0}}", 10, entity), std::format("{1:*>{0}}", 10, i));
		// 240*******
		ASSERT_EQ(std::format("{1:*<{0}}", 10, entity), std::format("{1:*<{0}}", 10, i));
		// ***240****
		ASSERT_EQ(std::format("{1:*^{0}}", 10, entity), std::format("{1:*^{0}}", 10, i));

		// *******f0
		ASSERT_EQ(std::format("{1:*>{0}x}", 10, entity), std::format("{1:*>{0}x}", 10, i));
		// f0********
		ASSERT_EQ(std::format("{1:*<{0}x}", 10, entity), std::format("{1:*<{0}x}", 10, i));
		// ****f0****
		ASSERT_EQ(std::format("{1:*^{0}x}", 10, entity), std::format("{1:*^{0}x}", 10, i));
	}
}

TEST(ECS, Entity_Formatting_Version) {
	ecs::Domain domain;

	for (u64 i = 0; i <= 100; ++i) {
		auto entity = domain.newEntity();
		auto entity1337 = ecs::_details::EntityTraits::Ent::fromParts(*entity, 1'337);

		// 240.1337
		ASSERT_EQ(std::format("{:v}", entity1337), std::format("{}.1337", i));
		// 240|1337
		ASSERT_EQ(std::format("{:v|}", entity1337), std::format("{}|1337", i));

		// f0|539
		ASSERT_EQ(std::format("{:xv|}", entity1337), std::format("{:x}|{:x}", i, 1'337));

		// *0xf|0x539
		ASSERT_EQ(std::format("{:*>#10xv|}", entity1337), std::format("{:*>10}", std::format("{:#x}|{:#x}", i, 1'337)));
		// 0xf|0x539*
		ASSERT_EQ(
			std::format("{:*<#{}xv|}", entity1337, 10),
			std::format("{:*<10}", std::format("{:#x}|{:#x}", i, 1'337))
		);
		// *0xf0|0x539
		ASSERT_EQ(
			std::format("{1:*^#{0}xv|}", 10, entity1337),
			std::format("{:*^10}", std::format("{:#x}|{:#x}", i, 1'337))
		);

		// 240.null
		ASSERT_EQ(
			std::format("{:v}", ecs::_details::EntityTraits::Ent::fromParts(*entity, ecs::nullVersion)),
			std::format("{}.null", entity)
		);
		// null.null
		ASSERT_EQ(std::format("{:v}", ecs::nullEntity), "null.null");
	}
}
