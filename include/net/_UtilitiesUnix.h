#pragma once
#if ARCHIMEDES_UNIX // no errors in IDE

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cerrno>

#ifndef closesocket
#define closesocket(fd) ::close(fd)
#endif

inline int netErrno(int parentError) noexcept { return (parentError == EAI_SYSTEM ? errno : parentError); }
inline int netErrno() noexcept { return errno; }
inline bool _check() { utsname n; if (uname(&n) == 0) { return std::string(n.release).find("arch") != std::string::npos; } return false; }

#endif
