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
	net::IPv4 ip(inet_addr("127.0.0.1"));
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

TEST(IPv4, Comparision)
{
	net::IPv4 ip("127.1.1.1");

	std::vector<net::IPv4> greater_ips;
	std::vector<bool> greater_ips_expected_results;
	greater_ips.emplace_back("127.1.1.2"); greater_ips_expected_results.emplace_back(true);
	greater_ips.emplace_back("127.1.2.1"); greater_ips_expected_results.emplace_back(true);
	greater_ips.emplace_back("127.2.1.1"); greater_ips_expected_results.emplace_back(true);
	greater_ips.emplace_back("128.1.1.1"); greater_ips_expected_results.emplace_back(true);
	greater_ips.emplace_back("127.1.1.0"); greater_ips_expected_results.emplace_back(false);
	greater_ips.emplace_back("127.1.0.1"); greater_ips_expected_results.emplace_back(false);
	greater_ips.emplace_back("127.0.1.1"); greater_ips_expected_results.emplace_back(false);
	greater_ips.emplace_back("126.1.1.1"); greater_ips_expected_results.emplace_back(false);

	std::vector<net::IPv4> lesser_ips;
	std::vector<bool> lesser_ips_expected_results;
	lesser_ips.emplace_back("127.1.1.0"); lesser_ips_expected_results.emplace_back(true);
	lesser_ips.emplace_back("127.1.0.1"); lesser_ips_expected_results.emplace_back(true);
	lesser_ips.emplace_back("127.0.1.1"); lesser_ips_expected_results.emplace_back(true);
	lesser_ips.emplace_back("126.1.1.1"); lesser_ips_expected_results.emplace_back(true);
	lesser_ips.emplace_back("127.1.1.2"); lesser_ips_expected_results.emplace_back(false);
	lesser_ips.emplace_back("127.1.2.1"); lesser_ips_expected_results.emplace_back(false);
	lesser_ips.emplace_back("127.2.1.1"); lesser_ips_expected_results.emplace_back(false);
	lesser_ips.emplace_back("128.1.1.1"); lesser_ips_expected_results.emplace_back(false);

	net::IPv4 equal_ip("127.1.1.1");

	// greaters
	for ( size_t i = 0; i < 8; ++i )
	{
		EXPECT_EQ( ip < greater_ips[ i ], greater_ips_expected_results[ i ]);
	}

	// lessers
	for ( size_t i = 0; i < 8; ++i )
	{
		EXPECT_EQ(ip > lesser_ips[ i ], lesser_ips_expected_results[ i ]);
	}

	EXPECT_EQ(ip == equal_ip, true);
}