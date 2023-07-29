#include <net/__net_init.hpp>
#include <net/exception.hpp>
#include <net/includes.hpp>

namespace arch::net {
__NetworkAuto::__NetworkAuto() {
#ifdef _WIN32
	WSADATA wsa_data;
	int init_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (init_result != 0) {
		throw NetException(gai_strerror(net_errno(init_result)));
	}
#endif
	initialized = true;
}
__NetworkAuto::~__NetworkAuto() {
#ifdef _WIN32
	WSACleanup();
#endif
}
}