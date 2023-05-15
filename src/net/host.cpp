#include <net/host.hpp>
#include <net/__net_init.hpp>
#include <cstring>
#include <algorithm>

#ifdef _WIN32
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined unix
#include <unistd.h>
#endif

namespace arch::net {
	Host::Host(IPv4 ip, bool update) {
		_ips[0] = ip;
		if (update) {
			this->update();
		}
	}
	Host::Host(std::string_view hostname) {
		_hostname = hostname;

		update();
	}
	Host Host::localhost(bool update) {
		return Host(IPv4::localhost, update);
	}
	const IPv4& Host::ip() const {
		return _ips[0];
	}
	const std::vector<IPv4>& Host::ips() const {
		return _ips;
	}
	bool Host::has_ip(IPv4 address) const {
		return std::find(_ips.begin(), _ips.end(), address) != _ips.end();
	}
	const std::string& Host::hostname() const {
		return _hostname;
	}
	bool Host::update() {
		if (not __net_auto.initialized) {
			// log error or smth
			return false;
		}
		
		std::string node_name;
		node_name.reserve(1025);
		memset(node_name.data(), 0, 1025);

		if (not _hostname.empty()) { // hostname avalible
			node_name = _hostname;
		}
		else if (_ips[0] == IPv4::localhost) {
			char hostname[NI_MAXHOST]{};
			gethostname(hostname, NI_MAXHOST);
			_hostname = hostname;
			node_name = _hostname;
		}
		else { // update hostname
			sockaddr_in sa;
			memset(&sa, 0, sizeof(sa));
			sa.sin_addr = _ips[0];
			sa.sin_family = AF_INET;
			sa.sin_port = htons(0);

			char hostname[NI_MAXHOST]{};
			char serv_info[NI_MAXSERV];

			int result = getnameinfo((sockaddr*)&sa, sizeof(sockaddr), hostname, NI_MAXHOST, serv_info, NI_MAXSERV, 0);
			if (result != 0) {
				// log error or smth WSAGetLastError()
			}
			else {
				_hostname = hostname;
			}

			node_name = _ips[0].str();
		}

		static addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		addrinfo* data;

		int result = getaddrinfo(node_name.c_str(), nullptr, &hints, &data);

		if (result != 0) {
			// log error or smth gai_strerror(result)
			return false;
		}

		for (auto i = data; i != nullptr; i = i->ai_next) {
			if (((sockaddr_in*)i->ai_addr)->sin_addr != _ips[0]) {
				_ips.emplace_back(((sockaddr_in*)i->ai_addr)->sin_addr);
			}
		}
		
		freeaddrinfo(data);

		return true;
	}
}