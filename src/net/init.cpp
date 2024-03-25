#include <net/Init.h>
#include <net/Exception.h>
#include <net/utilities.h>

namespace arch::net {
void Init::init() {
#if ARCHIMEDES_WINDOWS
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}
#endif
}
void Init::cleanup() {
#if ARCHIMEDES_WINDOWS
	int result = WSACleanup();
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}
#endif
}
}