#include <gtest/gtest.h>
#include <net/async/tcp_socket.hpp>
#include <net/exception.hpp>
#include <future>
#include <string>
#include <map>
#include <sstream>

TEST(AsyncTCPSocket, SendAndReceive) {
	namespace async = arch::net::async;
	auto&& msg = "przykladowa wiadomosc";
	
	auto timeout = std::chrono::milliseconds(3000);

	async::TCPSocket listen_sock;
#ifdef WIN32
	listen_sock.exclusive(false);
	EXPECT_FALSE(listen_sock.exclusive());
#endif
	listen_sock.reuse(true);
	EXPECT_TRUE(listen_sock.reuse());

	EXPECT_TRUE(listen_sock.bind(50420));
	EXPECT_TRUE(listen_sock.listen().get());
	
	async::TCPSocket recv_sock;
	auto accept_future = listen_sock.accept(recv_sock);

	async::TCPSocket send_sock;
#ifdef WIN32
	send_sock.exclusive(false);
	EXPECT_FALSE(send_sock.exclusive());
#endif
	send_sock.reuse(true);
	EXPECT_TRUE(send_sock.reuse());

	EXPECT_TRUE(send_sock.connect(async::Host::localhost(), 50420).get());
	EXPECT_TRUE(accept_future.get());

	char buf[100]{};
	auto recv_future = recv_sock.recv(buf, sizeof(buf), timeout.count());
	
	EXPECT_TRUE(send_sock.send(msg, sizeof(msg)).get());
	EXPECT_TRUE(recv_future.get());

	EXPECT_EQ(strcmp(buf, msg), 0);
}