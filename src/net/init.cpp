#include <net/init.hpp>
#include <net/exception.hpp>
#include <net/utilities.hpp>

namespace arch::net {
void Init::init() {
#ifdef WIN32
	WSADATA wsa_data;
	int init_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (init_result != 0) {
		throw NetException(gai_strerror(net_errno(init_result)));
	}
#endif
	_initialized = true;
}
void Init::cleanup() {
#ifdef WIN32
	int result = WSACleanup();
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}
#endif
}
bool Init::initialized() {
	return Init::_initialized;
}
}