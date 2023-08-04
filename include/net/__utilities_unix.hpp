#pragma once
#ifdef unix // no errors in IDE

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
#include <cerrno>

#ifndef closesocket(fd)
#define closesocket(fd) ::close(fd)
#endif

inline int net_errno(int parent_error) noexcept { return (parent_error == EAI_SYSTEM ? errno : parent_error); }
inline int net_errno() noexcept { return errno; }

#endif
