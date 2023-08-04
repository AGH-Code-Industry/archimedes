#pragma once
#ifdef WIN32 // no errors in IDE

#define NOMINMAX

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#ifndef poll
#define poll(fdarr, fds, timeout) WSAPoll(fdarr, fds, timeout)
#endif

inline int net_errno(int parent_error) noexcept { return WSAGetLastError(); }
inline int net_errno() noexcept { return WSAGetLastError(); }

#endif