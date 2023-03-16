#include <gtest/gtest.h>
#include <net/ipv4.hpp>
#include <sstream>

TEST(IPv4, Str2Bin)
{
	net::IPv4 ip;
	ip.str("127.0.0.1");
	EXPECT_EQ(ip.data.binary, inet_addr("127.0.0.1"));
}

TEST(IPv4, Oct2Bin)
{
	net::IPv4 ip(127, 0, 0, 1);
	EXPECT_EQ(ip.data.binary, inet_addr("127.0.0.1"));

	ip.data.octets[ 0 ] = 192;
	ip.data.octets[ 1 ] = 168;
	ip.data.octets[ 2 ] = 0;
	ip.data.octets[ 3 ] = 1;
	EXPECT_EQ(ip.data.binary, inet_addr("192.168.0.1"));
}

TEST(IPv4, Bin2Str)
{
	net::IPv4 ip = inet_addr("127.0.0.1");
	EXPECT_EQ(ip.str(), "127.0.0.1");
}

TEST(IPv4, Oct2Str)
{
	net::IPv4 ip(127, 0, 0, 1);
	EXPECT_EQ(ip.str(), "127.0.0.1");

	ip.data.octets[ 0 ] = 192;
	ip.data.octets[ 1 ] = 168;
	ip.data.octets[ 2 ] = 0;
	ip.data.octets[ 3 ] = 1;
	EXPECT_EQ(ip.str(), "192.168.0.1");
}

TEST(IPv4, OstreamPrint)
{
	net::IPv4 ip("192.168.0.1");
	std::stringstream s;
	s << ip;
	EXPECT_EQ(s.str(), ip.str());
}