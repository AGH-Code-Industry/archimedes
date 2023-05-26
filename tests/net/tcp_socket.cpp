#include <gtest/gtest.h>
#include <net/tcp_socket.hpp>
#include <future>

///*
TEST(TCPSocket, SendAndReceive) {
	namespace net = arch::net;
	auto&& msg = "przykladowa wiadomosc";

	auto timeout = std::chrono::seconds(3);

	auto listen_future = std::async(std::launch::async, [&]() {
		net::TCPSocket listen_sock;
#ifdef _WIN32
		listen_sock.exclusive(false);
		EXPECT_FALSE(listen_sock.exclusive());
#endif
		listen_sock.reuse(true);
		EXPECT_TRUE(listen_sock.reuse());

		EXPECT_TRUE(listen_sock.bind(50420));
		EXPECT_TRUE(listen_sock.listen());
		net::TCPSocket recv_sock;
		EXPECT_TRUE(listen_sock.accept(recv_sock));

		auto timer = std::chrono::system_clock::now() + timeout;
		for (;;) {
			if (std::chrono::system_clock::now() >= timer) {
				EXPECT_TRUE(false);
				return;
			}
			if (recv_sock.data_avalible()) {
				char buf[100]{};
				bool result = recv_sock.recv(buf, sizeof(buf));
				if (result) {
					EXPECT_EQ(strcmp(msg, buf), 0);
					return;
				}
				else {
					EXPECT_TRUE(false);
					return;
				}
			}
		}
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	net::TCPSocket send_sock;

#ifdef _WIN32
	send_sock.exclusive(false);
	EXPECT_FALSE(send_sock.exclusive());
#endif
	send_sock.reuse(true);
	EXPECT_TRUE(send_sock.reuse());

	EXPECT_TRUE(send_sock.connect(net::Host::localhost(), 50420));
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	EXPECT_TRUE(send_sock.send(msg, sizeof(msg)));

	listen_future.wait();
}
//*/