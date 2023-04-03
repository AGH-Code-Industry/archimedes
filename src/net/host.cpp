#include <net/host.hpp>
#include <net/__net_init.hpp>

#ifdef _WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined unix

#endif

namespace arch::net {
	Host::Host(IPv4 ip, bool get_hostname) {
		_ip = ip;
		if (get_hostname) {
			update_hostname();
		}
	}
	Host::Host(std::string_view hostname) {
		static addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		addrinfo* data;
		int result = getaddrinfo(hostname.data(), nullptr, &hints, &data);

		if (result != 0) {
			// log error or smth gai_strerror(result)
			return;
		}

		_ip = ((sockaddr_in*)data->ai_addr)->sin_addr;
		_hostname = hostname;

		for (auto i = data->ai_next; i != nullptr; i = i->ai_next) {
			_ips.emplace_back(((sockaddr_in*)i->ai_addr)->sin_addr);
		}

		freeaddrinfo(data);
	}
	Host Host::localhost() {
		return {IPv4::localhost, true};
	}
	const IPv4& Host::ip() const {
		return _ip;
	}
	const std::vector<IPv4>& Host::ips() const {
		return _ips;
	}
	const std::string& Host::hostname() const {
		return _hostname;
	}
	void Host::update_hostname() {
		if (not __net_auto.initialized) {
				// log error or smth
			const_cast<std::string&>(_hostname) = inet_ntoa(_ip);
			return;
		}

		sockaddr_in sa;
		memset(&sa, 0, sizeof(sa));
		sa.sin_addr = _ip;
		sa.sin_family = AF_INET;
		sa.sin_port = htons(0);

		char hostname[NI_MAXHOST];
		char serv_info[NI_MAXSERV];

		int result = getnameinfo((sockaddr*)&sa, sizeof(sockaddr), hostname, NI_MAXHOST, serv_info, NI_MAXSERV, NULL);
		if (result != 0) {
			// log error or smth WSAGetLastError()
			const_cast<std::string&>(_hostname) = inet_ntoa(_ip);
		}

		const_cast<std::string&>(_hostname) = hostname;
	}
}