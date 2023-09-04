#include <gtest/gtest.h>
#include <portability/byteswap.hpp>
#include <bit>

TEST(Portability, ByteSwap) {
	// in memory:
	// little-endian:	ef be ad de
	// big-endian:		de ad be ef
	auto deadbeef = 0xdeadbeef;
	arch::port::byteswap(deadbeef);
	EXPECT_EQ(deadbeef, 0xefbeadde);

	struct foo {
		long double val2;
		int val1;
		long int unsigned long val3;

		bool operator==(const foo&) const = default;
	} foo1{1, 420.1337, UINT_MAX}, foo2{foo1};
	// double byteswap
	EXPECT_EQ(arch::port::byteswap(arch::port::byteswap(foo1)), foo2);
}