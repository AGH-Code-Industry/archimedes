#include <algorithm>

#include <archimedes/net/GetMac.h>

#if ARCHIMEDES_UNIX

bool arch::net::getMac(std::array<arch::u8, 6>& result) noexcept {
	ifaddrs* ifaddr;
	if (getifaddrs(&ifaddr) == -1) {
		result = {};
		return false;
	}

	for (auto ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == nullptr || !(ifa->ifa_flags & IFF_UP)) {
			continue;
		}

		if (ifa->ifa_addr->sa_family == AF_PACKET) {
			sockaddr_ll* s = (sockaddr_ll*)ifa->ifa_addr;
			if (s->sll_halen == 6) {
				std::copy_n(s->sll_addr, 6, result.begin());
				if (result != std::array<arch::u8, 6>()) {
					freeifaddrs(ifaddr);
					return true;
				}
			}
		}
	}

	freeifaddrs(ifaddr);
	result = {};
	return false;
}

#endif
