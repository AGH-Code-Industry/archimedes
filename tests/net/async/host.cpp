#include <gtest/gtest.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#elif defined unix
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <net.hpp>
#include <iostream>
#include <net/async/host.hpp>

TEST(AsyncHost, LocalHost) {
	namespace net = arch::net;

	auto localhost_future = net::async::Host::localhost(true);
	auto localhost = localhost_future.get();

	EXPECT_EQ(localhost.ip(), net::IPv4::localhost);

	char buffer[256]{};
	gethostname(buffer, 256);

	EXPECT_EQ(strcmp(localhost.hostname().c_str(), buffer), 0);
}

TEST(AsyncHost, MyIPs) {
	namespace net = arch::net;

	std::vector<net::IPv4> expected_ips{
		// insert your IPs here
	};
	char buffer[256]{};
	gethostname(buffer, 256);
	auto this_host_future = net::async::Host::from_hostname(buffer);
	auto this_host = this_host_future.get();

	for (size_t i = 0; i != expected_ips.size(); ++i) {
		EXPECT_EQ(this_host.ips()[i], expected_ips[i]);
	}
}