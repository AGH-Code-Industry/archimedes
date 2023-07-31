#include <gtest/gtest.h>
#include <iostream>
#include <future>
#include <net/exception.hpp>

#include <net/udp_socket.hpp>

/*
TEST(UDPSocket, SendAndReceive) {
	namespace net = arch::net;
	auto&& msg = "przykladowa wiadomosc";

	auto timeout = std::chrono::seconds(3);
	auto future = std::async(std::launch::async, [&]() {
		net::UDPSocket receiver_sock;

#ifdef WIN32
		receiver_sock.exclusive(false);
		EXPECT_FALSE(receiver_sock.exclusive());
#endif
		receiver_sock.reuse(true);
		EXPECT_TRUE(receiver_sock.reuse());

		receiver_sock.bind(50420);

		auto timer = std::chrono::system_clock::now() + timeout;
		for (;;) {
			if (std::chrono::system_clock::now() >= timer) {
				EXPECT_TRUE(false);
				return;
			}
			if (receiver_sock.data_avalible()) {
				char buf[100]{};
				bool result = receiver_sock.recv(buf, sizeof(buf));
				if (result) {
					EXPECT_EQ(strcmp(msg, buf), 0);
					return;
				}
				else {
					EXPECT_TRUE(false);
					return;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	net::UDPSocket sender_sock;

#ifdef WIN32
	sender_sock.exclusive(false);
	EXPECT_FALSE(sender_sock.exclusive());
#endif
	sender_sock.reuse(true);
	EXPECT_TRUE(sender_sock.reuse());

	EXPECT_TRUE(sender_sock.bind(50420));

	EXPECT_TRUE(sender_sock.send_to(net::Host::localhost(), 50420, msg, sizeof(msg)));
	//while (future.wait_for(std::chrono::milliseconds(10)) != std::future_status::ready) {}

	future.wait();
}
*/