#include <gtest/gtest.h>
#include <net/utilities.hpp>
#include <net/init.hpp>
#include <net.hpp>
#include <iostream>

TEST(Host, LocalHost) { // ~15ms
    arch::net::Init::init();

    auto localhost = arch::net::Host::localhost(true);

    EXPECT_EQ(localhost.ip(), arch::net::IPv4::localhost);

    char buffer[256]{};
    gethostname(buffer, 256);

    EXPECT_EQ(strcmp(localhost.hostname().c_str(), buffer), 0);
}