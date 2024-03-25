#include <net/Host.h>
#include <net/Init.h>
#include <net/NetException.h>
#include <cstring>
#include <algorithm>
#include <net/Utilities.h>
#include <net/async/Host.h>
#include <net/IPv4.h>

namespace arch::net {
Host::Host(IPv4 ip, bool update) {
	_ips[0] = ip;
	if (update) {
		this->update();
	}
}
Host::Host(const std::string& hostname) {
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
bool Host::hasIp(IPv4 address) const {
	return std::find(_ips.begin(), _ips.end(), address) != _ips.end();
}
const std::string& Host::hostname() const {
	return _hostname;
}
std::string Host::_updateHostname() {
	std::string nodeName;
	nodeName.reserve(1025);
	memset(nodeName.data(), 0, 1025);

	if (not _hostname.empty()) { // hostname avalible
		nodeName = _hostname;
	}
	else if (_ips[0] == IPv4::localhost) {
		char hostname[NI_MAXHOST]{};
		gethostname(hostname, NI_MAXHOST);
		_hostname = hostname;
		nodeName = _hostname;
	}
	else { // update hostname
		sockaddr_in sa;
		memset(&sa, 0, sizeof(sa));
		sa.sin_addr = _ips[0];
		sa.sin_family = AF_INET;
		sa.sin_port = htons(0);

		char hostname[NI_MAXHOST]{};
		static char servInfo[NI_MAXSERV];

		int result = getnameinfo((sockaddr*)&sa, sizeof(sockaddr), hostname, NI_MAXHOST, servInfo, NI_MAXSERV, 0);
		if (result != 0) {
			throw NetException(gai_strerror(netErrno(result)));
		}
		else {
			_hostname = hostname;
		}

		nodeName = _ips[0].str();
	}

	return nodeName;
}
bool Host::update() {
	auto nodeName = _updateHostname();

	addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	addrinfo* data;
	int result = getaddrinfo(nodeName.c_str(), nullptr, &hints, &data);

	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}

	for (auto i = data; i != nullptr; i = i->ai_next) {
		if (((sockaddr_in*)i->ai_addr)->sin_addr != _ips[0]) {
			_ips.emplace_back(((sockaddr_in*)i->ai_addr)->sin_addr);
		}
	}

	freeaddrinfo(data);

	return true;
}
async::Host Host::async() const {
	async::Host host;
	host._ips = this->_ips;
	host._hostname = this->_hostname;
	return host;
}
Host::Host() {
	_ips[0] = IPv4();
}
}