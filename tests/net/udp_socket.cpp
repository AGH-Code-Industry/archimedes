#include <gtest/gtest.h>
#include <iostream>

#include <net/udp_socket.hpp>

namespace net = arch::net;

///*
TEST(UDPSocket, Sender) {
	net::UDPSocket sender_sock;
	sender_sock.bind(30420);
	net::Host host(net::IPv4("192.168.0.173"));
	EXPECT_TRUE(sender_sock.send_to(host, "przykladowa wiadomosc"));
	std::cout << "Type 'Y' if receiver got the message: ";
	char ans;
	std::cin >> ans;
	EXPECT_TRUE(tolower(ans) == 'y');
}
//*/

/*
TEST(UDPSocket, Receiver) {
	net::UDPSocket receiver_sock;
	receiver_sock.bind(30420);
	for (;;) {
		if (receiver_sock.data_avalible()) {
			char buf[1024]{};
			auto result = receiver_sock.recv_from(buf, sizeof(buf));
			if (result.ip() != net::IPv4((net::IPv4::binary_type)0)) {
				std::cout << "Received message from: " << result.ip() << ":\n\t" << buf << '\n';
				std::cout << "Type 'Y' if receiver got the correct message: ";
				char ans;
				std::cin >> ans;
				EXPECT_TRUE(tolower(ans) == 'y');
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}
*/