#include <archimedes/net/Init.h>
#include <archimedes/net/NetException.h>
#include <archimedes/net/Utilities.h>

namespace arch::net {

void Init::init() {
#if ARCHIMEDES_WINDOWS
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}
#endif
}

void Init::cleanup() {
#if ARCHIMEDES_WINDOWS
	int result = WSACleanup();
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}
#endif
}

} // namespace arch::net
