#include <gtest/gtest.h>
#include <net/tcp_socket.hpp>
#include <net/exception.hpp>
#include <future>
#include <string>
#include <map>
#include <sstream>

///*
TEST(TCPSocket, SendAndReceive) {
	namespace net = arch::net;
	auto&& msg = "przykladowa wiadomosc";

	auto timeout = std::chrono::seconds(3);

	auto listen_future = std::async(std::launch::async, [&]() {
		net::TCPSocket listen_sock;
		try {
#ifdef _WIN32
			listen_sock.exclusive(false);
			EXPECT_FALSE(listen_sock.exclusive());
#endif
			listen_sock.reuse(true);
			EXPECT_TRUE(listen_sock.reuse());
		}
		catch (std::exception& e) {
			std::cout << "first " << e.what() << '\n';
		}

		
		net::TCPSocket recv_sock;
		EXPECT_TRUE(listen_sock.bind(50420));
		EXPECT_TRUE(listen_sock.listen());
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

namespace net = arch::net;
using namespace std::string_literals;

std::map<std::string, std::string> database
{
	{
	{"Mariusz Pudzianowski"s, "Polska gurom!!!1!"s},
	{"Red Impostor"s, "sus ,:/"s},
	{"Ben Christie"s, "bigounce"s},
	{"Chemia UJ"s, "prawnik zalecil abym zaniechal zartu"s},
	{"Akademik Kapitol"s, "rzadowy program winda+"s}
	}
};

auto accept_condition = [](void* data, int data_len, void* additional_data, void* response_buffer, int response_len) -> bool {
	std::string username_password{(const char*)data, (size_t)data_len};
	auto&& user_database = *(std::map<std::string, std::string>*)additional_data;
	unsigned char resp = 0;

	auto&& delim_pos = username_password.find('|');
	auto&& end_pos = username_password.find('|', delim_pos + 1);
	if (delim_pos == std::string_view::npos or end_pos == std::string_view::npos) {
		// bad format
		resp = 1 << 2;
	}
	if (resp == 0) {
		std::string username = username_password.substr(0, delim_pos);
		std::string password = username_password.substr(delim_pos + 1, end_pos - delim_pos - 1);
		auto&& found_user = user_database.find(username);
		if (found_user == user_database.end()) {
			// user not found
			resp = 1;
		}
		if (resp == 0) {
			if (found_user->second != password) {
				// wrong password
				resp = 1 << 1;
			}
		}
	}
	memcpy(response_buffer, &resp, sizeof(resp));
	return resp == 0;
};
auto response_handler = [](void* response, int response_len, void* additional_data) -> bool {
	unsigned char resp{};
	memcpy(&resp, response, sizeof(resp));
	std::ostream& stream = *(std::ostream*)additional_data;
	if (resp != 0) {
		std::string message;
		switch (resp) {
			case 1:
				message = "username not found";
				break;
			case 1 << 1:
				message = "wrong password";
				break;
			case 1 << 2:
				message = "bad format";
				break;
			default:
				message = "unknown error";
				break;
		}
		stream << message << '\n';
		return false;
	}
	else {
		return true;
	}
};

TEST(TCPSocket, ConditionalConnectSuccess) {
	///*
	auto future = std::async(std::launch::async, [&]() {
		net::TCPSocket listen_sock;
#ifdef _WIN32
		listen_sock.exclusive(false);
		EXPECT_FALSE(listen_sock.exclusive());
#endif
		listen_sock.reuse(true);
		EXPECT_TRUE(listen_sock.reuse());
		listen_sock.bind(50420);
		EXPECT_TRUE(listen_sock.listen());

		net::TCPSocket connsock;
		try {
			EXPECT_TRUE(listen_sock.cond_accept(connsock, accept_condition, 128, 1, &database));
		}
		catch (std::exception& e) {
			std::cout << e.what() << '\n';
		}

		return;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//*/
	net::TCPSocket conn_sock;
#ifdef _WIN32
	conn_sock.exclusive(false);
	EXPECT_FALSE(conn_sock.exclusive());
#endif
	conn_sock.reuse(true);
	EXPECT_TRUE(conn_sock.reuse());

	srand(time(nullptr));

	int userid = rand() % 5;
	auto iter = database.begin();
	for (int i = 0; i != userid; ++i) {
		++iter;
	}

	std::string data = iter->first + '|' + iter->second + '|';

	EXPECT_TRUE(conn_sock.cond_connect(net::Host::localhost(), 50420, data.data(), data.length(), 1, response_handler));

	future.wait();
}

TEST(TCPSocket, ConditionalConnectUserNotFound) {
	///*
	auto future = std::async(std::launch::async, [&]() {
		net::TCPSocket listen_sock;
#ifdef _WIN32
		listen_sock.exclusive(false);
		EXPECT_FALSE(listen_sock.exclusive());
#endif
		listen_sock.reuse(true);
		EXPECT_TRUE(listen_sock.reuse());
		listen_sock.bind(50420);
		EXPECT_TRUE(listen_sock.listen());

		net::TCPSocket connsock;
		EXPECT_THROW(listen_sock.cond_accept(connsock, accept_condition, 128, 1, &database), arch::NetException);

		return;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//*/
	net::TCPSocket conn_sock;
#ifdef _WIN32
	conn_sock.exclusive(false);
	EXPECT_FALSE(conn_sock.exclusive());
#endif
	conn_sock.reuse(true);
	EXPECT_TRUE(conn_sock.reuse());

	srand(time(nullptr));

	int userid = rand() % 5;
	auto iter = database.begin();
	for (int i = 0; i != userid; ++i) {
		++iter;
	}

	std::string data = iter->first + 'X' + '|' + iter->second + '|';

	std::stringstream sstream;

	EXPECT_THROW(conn_sock.cond_connect(net::Host::localhost(), 50420, data.data(), data.length(), 1, response_handler, &sstream), arch::NetException);

	EXPECT_EQ(sstream.str(), "username not found\n");

	future.wait();
}

TEST(TCPSocket, ConditionalConnectWrongPassword) {
	///*
	auto future = std::async(std::launch::async, [&]() {
		net::TCPSocket listen_sock;
#ifdef _WIN32
		listen_sock.exclusive(false);
		EXPECT_FALSE(listen_sock.exclusive());
#endif
		listen_sock.reuse(true);
		EXPECT_TRUE(listen_sock.reuse());
		listen_sock.bind(50420);
		EXPECT_TRUE(listen_sock.listen());

		net::TCPSocket connsock;
		EXPECT_THROW(listen_sock.cond_accept(connsock, accept_condition, 128, 1, &database), arch::NetException);

		return;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//*/
	net::TCPSocket conn_sock;
#ifdef _WIN32
	conn_sock.exclusive(false);
	EXPECT_FALSE(conn_sock.exclusive());
#endif
	conn_sock.reuse(true);
	EXPECT_TRUE(conn_sock.reuse());

	srand(time(nullptr));

	int userid = rand() % 5;
	auto iter = database.begin();
	for (int i = 0; i != userid; ++i) {
		++iter;
	}

	std::string data = iter->first + '|' + iter->second + 'X' + '|';

	std::stringstream sstream;

	EXPECT_THROW(conn_sock.cond_connect(net::Host::localhost(), 50420, data.data(), data.length(), 1, response_handler, &sstream), arch::NetException);

	EXPECT_EQ(sstream.str(), "wrong password\n");

	future.wait();
}

TEST(TCPSocket, ConditionalConnectBadFormat) {
	///*
	auto future = std::async(std::launch::async, [&]() {
		net::TCPSocket listen_sock;
#ifdef _WIN32
		listen_sock.exclusive(false);
		EXPECT_FALSE(listen_sock.exclusive());
#endif
		listen_sock.reuse(true);
		EXPECT_TRUE(listen_sock.reuse());
		listen_sock.bind(50420);
		EXPECT_TRUE(listen_sock.listen());

		net::TCPSocket connsock;
		EXPECT_THROW(listen_sock.cond_accept(connsock, accept_condition, 128, 1, &database), arch::NetException);

		return;
	});
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	//*/
	net::TCPSocket conn_sock;
#ifdef _WIN32
	conn_sock.exclusive(false);
	EXPECT_FALSE(conn_sock.exclusive());
#endif
	conn_sock.reuse(true);
	EXPECT_TRUE(conn_sock.reuse());

	srand(time(nullptr));

	int userid = rand() % 5;
	auto iter = database.begin();
	for (int i = 0; i != userid; ++i) {
		++iter;
	}

	std::string data = iter->first + '|' + iter->second;

	std::stringstream sstream;

	EXPECT_THROW(conn_sock.cond_connect(net::Host::localhost(), 50420, data.data(), data.length(), 1, response_handler, &sstream), arch::NetException);

	EXPECT_EQ(sstream.str(), "bad format\n");

	future.wait();
}