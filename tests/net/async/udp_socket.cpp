#include <gtest/gtest.h>
#include <iostream>
#include <net/async/udp_socket.hpp>

TEST(AsyncUDPSocket, SendAndReceive) {
	namespace net = arch::net;
	
	auto&& msg = "przykladowa wiadomosc";
	auto timeout = std::chrono::seconds(3);
	
	net::async::UDPSocket receiver_sock;
#ifdef WIN32
	receiver_sock.exclusive(false);
	EXPECT_FALSE(receiver_sock.exclusive());
#endif
	receiver_sock.reuse(true);
	EXPECT_TRUE(receiver_sock.reuse());

	receiver_sock.bind(50420);

	char buf[100]{};
	auto recv_future = receiver_sock.recv(buf, sizeof(buf));

	net::async::UDPSocket sender_sock;
#ifdef WIN32
	sender_sock.exclusive(false);
	EXPECT_FALSE(sender_sock.exclusive());
#endif
	sender_sock.reuse(true);
	EXPECT_TRUE(sender_sock.reuse());

	sender_sock.bind(50420);
	//getchar();
	auto send_future = sender_sock.send_to(net::async::Host::localhost(), 50420, msg, sizeof(msg));

	EXPECT_TRUE(send_future.get());
	EXPECT_TRUE(recv_future.get());
	EXPECT_EQ(strcmp(msg, buf), 0);
}