#include <gtest/gtest.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <net.hpp>
#include <iostream>

TEST(Host, LocalHost) { // ~15ms
    auto localhost = arch::net::Host::localhost();

    EXPECT_EQ(localhost.ip(), arch::net::IPv4::localhost);

    char buffer[256]{};
    gethostname(buffer, 256);
    EXPECT_EQ(strcmp(localhost.hostname().c_str(), buffer), 0);
}

TEST(Host, MyIPs) { // ~2ms
    std::vector<arch::net::IPv4> expected_ips{
        // insert your IPs here
    };
    char buffer[256]{};
    gethostname(buffer, 256);
    auto this_host = arch::net::Host(buffer);

    for (size_t i = 0; i != expected_ips.size(); ++i) {
        EXPECT_EQ(this_host.ips()[i], expected_ips[i]);
    }
}

TEST(Host, OtherHost) { // ~110ms
    // resolving IP from android phones does not work, idk why
    std::string_view hostname = "DESKTOP-22S4TN1";
    arch::net::IPv4 expected_ip{"192.168.0.193"};
    auto other_host = arch::net::Host(hostname);

    if (other_host.hostname().empty()) {
        EXPECT_TRUE(false) << "Host not found\n";
        return;
    }

    EXPECT_EQ(expected_ip, other_host.ip());
}

// other tests lack any sense, because of difference between writer's and tester's network structure