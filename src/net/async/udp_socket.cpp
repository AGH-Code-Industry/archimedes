#include <net/async/udp_socket.hpp>
#include <net/exception.hpp>

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
		return std::async(std::launch::async, [this](net::Host h, port_type p, const char* d, int l)->bool {
			std::lock_guard lock(_send_mutex);
			return _base::send_to(h, p, d, l);
		}, host.sync(), port, data, len);
	}
	std::future<bool> UDPSocket::send_to(const Host& host, const char* data, int len) {
		return send_to(host, _port, data, len);
	}
	std::future<bool> UDPSocket::send_to(const Host& host, port_type port, const std::string& data) {
		return send_to(host, port, data.data(), data.length());
	}
	std::future<bool> UDPSocket::send_to(const Host& host, const std::string& data) {
		return send_to(host, _port, data);
	}

	std::future<bool> UDPSocket::recv(char* buf, int buflen, int& length, timeout_t timeout, bool peek) {
		return std::async(std::launch::async, [this](char* b, int bl, std::reference_wrapper<int> l, timeout_t t, bool p)->bool {
			std::unique_lock lock(_recv_mutex, std::defer_lock);
			std::chrono::milliseconds time_left;
			if (t < 0) { // timeout == inf
				lock.lock();
				time_left = std::chrono::milliseconds(t);
			}
			else { // timeout != inf
				auto timer = std::chrono::high_resolution_clock::now();
				if (lock.try_lock_for(std::chrono::milliseconds(t))) {
					time_left = std::chrono::milliseconds(t) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timer);
				}
				else {
					return false;
				}
			}

			pollfd poll_data{};
			poll_data.fd = _socket;
			poll_data.events = POLLRDNORM;
			poll_data.revents = 0;
			int result = poll(&poll_data, 1, time_left.count());

			if (result == SOCKET_ERROR) {
				throw NetException(gai_strerror(net_errno()));
			}
			else if (result == 0) { // timeout expired
				return false;
			}
			else if (poll_data.revents & POLLRDNORM) { // read
				return _base::recv(b, bl, l.get(), p);
			}
			else { // cannot read
				return false;
			}
		}, buf, buflen, std::ref(length), timeout, peek);
	}
	std::future<bool> UDPSocket::recv(char* buf, int buflen, int& length, bool peek) {
		return recv(buf, buflen, length, -1, peek);
	}
	std::future<bool> UDPSocket::recv(char* buf, int buflen, timeout_t timeout, bool peek) {
		int temp;
		return recv(buf, buflen, temp, timeout, peek);
	}
	std::future<bool> UDPSocket::recv(char* buf, int buflen, bool peek) {
		return recv(buf, buflen, -1, peek);
	}
	
	std::future<Host> UDPSocket::recv_from(char* buf, int buflen, int& length, timeout_t timeout, bool peek) {
		return std::async(std::launch::async, [this](char* b, int bl, std::reference_wrapper<int> l, timeout_t t, bool p)->Host {
			std::unique_lock lock(_recv_mutex, std::defer_lock);
			std::chrono::milliseconds time_left;
			if (t < 0) { // timeout == inf
				lock.lock();
				time_left = std::chrono::milliseconds(t);
			}
			else { // timeout != inf
				auto timer = std::chrono::high_resolution_clock::now();
				if (lock.try_lock_for(std::chrono::milliseconds(t))) {
					time_left = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timer);
				}
				else {
					return Host(IPv4());
				}
			}

			pollfd poll_data{};
			poll_data.fd = _socket;
			poll_data.events = POLLRDNORM;
			poll_data.revents = 0;
			int result = poll(&poll_data, 1, time_left.count());

			if (result == SOCKET_ERROR) {
				throw NetException(gai_strerror(net_errno()));
			}
			else if (result == 0) { // timeout expired
				return Host(IPv4());
			}
			else if (poll_data.revents & POLLRDNORM) { // read
				return _base::recv_from(b, bl, l.get(), p).async();
			}
			else { // cannot read
				return Host(IPv4());
			}
		}, buf, buflen, std::ref(length), timeout, peek);
	}
	std::future<Host> UDPSocket::recv_from(char* buf, int buflen, int& length, bool peek) {
		return recv_from(buf, buflen, length, -1, peek);
	}
	std::future<Host> UDPSocket::recv_from(char* buf, int buflen, timeout_t timeout, bool peek) {
		int temp;
		return recv_from(buf, buflen, temp, timeout, peek);
	}
	std::future<Host> UDPSocket::recv_from(char* buf, int buflen, bool peek) {
		return recv_from(buf, buflen, -1, peek);
	}
}