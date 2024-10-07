#include <bitset>

#include <gtest/gtest.h>
#include <Net.h>

TEST(IPv4Mask, Get) {
	arch::net::IPv4Mask mask(23);

	auto&& data = mask.data();

	EXPECT_EQ(data.octets[0], 255);
	EXPECT_EQ(data.octets[1], 255);
	EXPECT_EQ(data.octets[2], 254);
	EXPECT_EQ(data.octets[3], 0);
}

TEST(IPv4Mask, Invalid) {
	EXPECT_THROW(arch::net::IPv4Mask mask1("255.255.255.2"), arch::net::Exception);
	EXPECT_THROW(arch::net::IPv4Mask mask2("255.255.2.255"), arch::net::Exception);
	EXPECT_THROW(arch::net::IPv4Mask mask3("255.2.255.255"), arch::net::Exception);
	EXPECT_THROW(arch::net::IPv4Mask mask4("2.255.255.255"), arch::net::Exception);
}

TEST(IPv4Network, Constructors) {
	arch::net::IPv4Network net1(arch::net::IPv4("192.168.0.0"), arch::net::IPv4Mask(24));
	arch::net::IPv4Network net2(arch::net::IPv4("192.168.0.0"), 24);
	arch::net::IPv4Network net3("192.168.0.0/24");

	EXPECT_EQ(net1.networkAddress(), net2.networkAddress());
	EXPECT_EQ(net2.networkAddress(), net3.networkAddress());
	EXPECT_EQ(net1.networkAddress(), net3.networkAddress());

	EXPECT_EQ(net1.networkAddress(), arch::net::IPv4("192.168.0.0"));
	EXPECT_EQ(net1.mask(), arch::net::IPv4Mask(24));
}

TEST(IPv4Network, Broadcast) {
	arch::net::IPv4Network net1("192.168.0.0/24");
	arch::net::IPv4Network net2("0.0.0.0/0");

	auto&& mask = net2.mask().data();
	auto&& addr = net2.networkAddress().data();

	EXPECT_EQ(net1.broadcast(), arch::net::IPv4("192.168.0.255"));
	EXPECT_EQ(net2.broadcast(), arch::net::IPv4("255.255.255.255"));
}

TEST(IPv4Network, FirstHost) {
	arch::net::IPv4Network net1("192.168.0.8/30");
	arch::net::IPv4Network net2("192.168.0.8/29");

	EXPECT_EQ(net1.firstHost(), arch::net::IPv4("192.168.0.9"));
	EXPECT_EQ(net2.firstHost(), arch::net::IPv4("192.168.0.9"));
}

TEST(IPv4Network, LastHost) {
	arch::net::IPv4Network net1("192.168.0.8/30");
	arch::net::IPv4Network net2("192.168.0.8/29");

	EXPECT_EQ(net1.lastHost(), arch::net::IPv4("192.168.0.10"));
	EXPECT_EQ(net2.lastHost(), arch::net::IPv4("192.168.0.14"));
}

TEST(IPv4Network, NetworkAddress) {
	arch::net::IPv4Network net1("192.168.0.8/30");
	arch::net::IPv4Network net2("192.168.0.8/29");

	EXPECT_EQ(net1.networkAddress(), arch::net::IPv4("192.168.0.8"));
	EXPECT_EQ(net2.networkAddress(), arch::net::IPv4("192.168.0.8"));
}

TEST(IPv4Network, Mask) {
	arch::net::IPv4Network net1("192.168.0.8/30");
	arch::net::IPv4Network net2("192.168.0.8/29");

	EXPECT_EQ(net1.mask(), arch::net::IPv4Mask(30));
	EXPECT_EQ(net2.mask(), arch::net::IPv4Mask(29));
}

TEST(IPv4Network, NetworkFromHost) {
	arch::net::IPv4Network net1("192.168.0.200/24");

	EXPECT_EQ(net1.mask(), arch::net::IPv4Mask(24));
	EXPECT_EQ(net1.networkAddress(), arch::net::IPv4("192.168.0.0"));
}
