#pragma once
#if ARCHIMEDES_WINDOWS // no errors in IDE

#define NOMINMAX

#include <WS2tcpip.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#ifndef poll
#define poll WSAPoll
#endif

inline int netErrno(int _) noexcept {
	return WSAGetLastError();
}

inline int netErrno() noexcept {
	return WSAGetLastError();
}

inline bool _check() {
	return false;
}

#endif
