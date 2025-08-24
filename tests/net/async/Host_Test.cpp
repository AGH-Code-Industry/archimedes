#include <iostream>

#include <archimedes/Net.h>
#include <gtest/gtest.h>

TEST(AsyncHost, LocalHost) {
	namespace net = arch::net;

	net::Init::init();

	auto localhost_future = net::async::Host::localhost(true);
	auto localhost = localhost_future.get().first;

	EXPECT_EQ(localhost.ip(), net::IPv4::localhost);

	char buffer[256]{};
	gethostname(buffer, 256);

	EXPECT_EQ(strcmp(localhost.hostname().c_str(), buffer), 0);
}
