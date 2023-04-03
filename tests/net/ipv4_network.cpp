#include <gtest/gtest.h>
#include <net/ipv4_network.hpp>
#include <bitset>
#include <iostream>

TEST(IPv4Mask, Get) {
	arch::net::IPv4Mask mask(23);

	auto&& data = mask.data();

	EXPECT_EQ(data.octets[0], 255);
	EXPECT_EQ(data.octets[1], 255);
	EXPECT_EQ(data.octets[2], 254);
	EXPECT_EQ(data.octets[3], 0);
}

TEST(IPv4Mask, Invalid) {
	arch::net::IPv4Mask mask1("255.255.255.2");
	arch::net::IPv4Mask mask2("255.255.2.255");
	arch::net::IPv4Mask mask3("255.2.255.255");
	arch::net::IPv4Mask mask4("2.255.255.255");

	EXPECT_EQ(mask1, arch::net::IPv4Mask::invalid);
	EXPECT_EQ(mask2, arch::net::IPv4Mask::invalid);
	EXPECT_EQ(mask3, arch::net::IPv4Mask::invalid);
	EXPECT_EQ(mask4, arch::net::IPv4Mask::invalid);
}

TEST(IPv4Network, Constructors) {
	arch::net::IPv4Network net1(arch::net::IPv4("192.168.0.0"), arch::net::IPv4Mask(24));
	arch::net::IPv4Network net2(arch::net::IPv4("192.168.0.0"), 24);
	arch::net::IPv4Network net3("192.168.0.0/24");

	EXPECT_EQ(net1.network_address(), net2.network_address());
	EXPECT_EQ(net2.network_address(), net3.network_address());
	EXPECT_EQ(net1.network_address(), net3.network_address());

	EXPECT_EQ(net1.network_address(), arch::net::IPv4("192.168.0.0"));
	EXPECT_EQ(net1.mask(), arch::net::IPv4Mask(24));
}

TEST(IPv4Network, Broadcast) {
	arch::net::IPv4Network net1("192.168.0.0/24");
	arch::net::IPv4Network net2("0.0.0.0/0");

	auto&& mask = net2.mask().data();
	auto&& addr = net2.network_address().data();

	EXPECT_EQ(net1.broadcast(), arch::net::IPv4("192.168.0.255"));
	EXPECT_EQ(net2.broadcast(), arch::net::IPv4("255.255.255.255"));
}

TEST(IPv4Network, FirstHost) {
	arch::net::IPv4Network net1("192.168.0.8/30");
	arch::net::IPv4Network net2("192.168.0.8/29");

	EXPECT_EQ(net1.first_host(), arch::net::IPv4("192.168.0.9"));
	EXPECT_EQ(net2.first_host(), arch::net::IPv4("192.168.0.9"));
}

TEST(IPv4Network, LastHost) {
	arch::net::IPv4Network net1("192.168.0.8/30");
	arch::net::IPv4Network net2("192.168.0.8/29");

	EXPECT_EQ(net1.last_host(), arch::net::IPv4("192.168.0.10"));
	EXPECT_EQ(net2.last_host(), arch::net::IPv4("192.168.0.14"));
}

TEST(IPv4Network, NetworkAddress) {
	arch::net::IPv4Network net1("192.168.0.8/30");
	arch::net::IPv4Network net2("192.168.0.8/29");

	EXPECT_EQ(net1.network_address(), arch::net::IPv4("192.168.0.8"));
	EXPECT_EQ(net2.network_address(), arch::net::IPv4("192.168.0.8"));
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
	EXPECT_EQ(net1.network_address(), arch::net::IPv4("192.168.0.0"));
}