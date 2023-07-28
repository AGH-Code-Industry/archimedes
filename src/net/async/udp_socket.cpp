#include <net/async/udp_socket.hpp>

namespace arch::net::async {
	UDPSocket::UDPSocket() :
		_base() {}
	UDPSocket::UDPSocket(port_type port) :
		_base(port) {}
	UDPSocket::UDPSocket(IPv4 address, port_type port) :
		_base(address, port) {}
	UDPSocket::~UDPSocket() {
		_base::~UDPSocket();
	}
	
	std::future<bool> UDPSocket::send_to(const Host& host, port_type port, const char* data, int len) {
		return std::async(std::launch::async, [this](std::reference_wrapper<const Host> h, port_type p, const char* d, int l)->bool {
			std::lock_guard lock(_send_mutex);
			return _base::send_to(h.get().sync(), p, d, l);
		}, std::cref(host), port, data, len);
	}
	std::future<bool> UDPSocket::send_to(const Host& host, const char* data, int len) {
		return send_to(host, _port, data, len);
	}
	std::future<bool> UDPSocket::send_to(const Host& host, port_type port, std::string_view data) {
		return send_to(host, port, data.data(), data.length());
	}
	std::future<bool> UDPSocket::send_to(const Host& host, std::string_view data) {
		return send_to(host, _port, data);
	}

	std::future<bool> UDPSocket::recv(char* buf, int buflen, int& length, timeout_t timeout, bool peek) {
		return std::async(std::launch::async, [this](char* b, int bl, std::reference_wrapper<int> l, timeout_t t, bool p)->bool {
			// time this
		}, buf, buflen, std::ref(length), timeout, peek);
	}
}