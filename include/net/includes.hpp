#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
inline auto poll(LPWSAPOLLFD fdArray, ULONG fds, INT timeout) { return WSAPoll(fdArray, fds, timeout); }
#elif defined unix
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif