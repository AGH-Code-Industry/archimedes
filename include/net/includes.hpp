#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#ifndef __ARCH_NET_WIN32_POLL_ALIAS__
#define __ARCH_NET_WIN32_POLL_ALIAS__
inline auto poll(LPWSAPOLLFD fdArray, ULONG fds, INT timeout) { return WSAPoll(fdArray, fds, timeout); }
#endif//__ARCH_NET_WIN32_POLL_ALIAS__
#elif defined unix
#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#endif
#include <chrono>
namespace arch::net {
#ifndef __NET_ERRNO__
#define __NET_ERRNO__
inline int net_errno(int parent_error) noexcept {
	return
#ifdef WIN32
		WSAGetLastError();
#elif defined unix
	(parent_error == EAI_SYSTEM ? errno : parent_error);
#endif
}
inline int net_errno() noexcept {
	return
#ifdef WIN32
		WSAGetLastError();
#elif defined unix
		errno;
#endif
}
#endif//__NET_ERRNO__
/// @brief Timeout type, milliseconds.
///
using timeout_t = std::chrono::milliseconds::rep;
}