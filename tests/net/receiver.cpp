#include <gtest/gtest.h>
#include <iostream>

#include <net/includes.hpp>

/*TEST(Receiver, Main) {
	WSAData data;

	int i_result;

	// Initialize Winsock
	i_result = WSAStartup(MAKEWORD(2, 2), &data);
	EXPECT_EQ(i_result, 0);

	auto sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);


	if (sock == INVALID_SOCKET) {
		std::cout << WSAGetLastError() << '\n';
		closesocket(sock);
		WSACleanup();
		EXPECT_NE(sock, INVALID_SOCKET);
	}

	sockaddr_in si_this{};
	sockaddr_in si_other{};
	int s_len = sizeof(si_other);
	memset(&si_this, 0, sizeof(si_this));
	si_this.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	si_this.sin_port = htons(30420);
	si_this.sin_family = AF_INET;

	auto result = bind(sock, (sockaddr*)&si_this, sizeof(si_this));

	if (result != 0) {
		std::cout << WSAGetLastError() << '\n';
		closesocket(sock);
		WSACleanup();
		EXPECT_EQ(result, 0);
	}

	char buffer[1024]{};

	for (;;) {
		if (recvfrom(sock, buffer, sizeof(buffer) / sizeof(*buffer), 0, (sockaddr*)&si_other, &s_len) > 0) {
			std::cout << "Received message: " << buffer << '\n';
			closesocket(sock);
			WSACleanup();
			ASSERT_TRUE(true);
			break;
		}
	}
}*/