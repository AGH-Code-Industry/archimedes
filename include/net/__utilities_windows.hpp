#pragma once
#ifdef WIN32 // no errors in IDE

#define NOMINMAX

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

inline auto poll(LPWSAPOLLFD fdArray, ULONG fds, INT timeout) { return WSAPoll(fdArray, fds, timeout); }
inline int net_errno(int parent_error) noexcept { return WSAGetLastError(); }
inline int net_errno() noexcept { return WSAGetLastError(); }

#endif