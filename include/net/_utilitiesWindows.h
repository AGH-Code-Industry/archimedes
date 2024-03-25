#pragma once
#if ARCHIMEDES_WINDOWS // no errors in IDE

#define NOMINMAX

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#ifndef poll
#define poll WSAPoll
#endif

inline int net_errno(int _ignored) noexcept { return WSAGetLastError(); }
inline int net_errno() noexcept { return WSAGetLastError(); }
inline bool _check() { return false; }

#endif