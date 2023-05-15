#include <net/__net_init.hpp>
#ifdef _WIN32
#include <WinSock2.h>
#elif defined unix

#endif

namespace arch::net {
	__NetworkAuto::__NetworkAuto() {
#ifdef _WIN32
		WSADATA wsa_data;
		int init_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
		if (init_result != 0) {
			// log error or smth
			return;
		}
#elif defined unix
		// nothing
#endif
		initialized = true;
	}
	__NetworkAuto::~__NetworkAuto() {
#ifdef _WIN32
		WSACleanup();
#elif defined unix
		// nothing
#endif
	}
}