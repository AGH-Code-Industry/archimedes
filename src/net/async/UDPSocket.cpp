#include <archimedes/net/NetException.h>
#include <archimedes/net/async/UDPSocket.h>

namespace arch::net::async {

UDPSocket::UDPSocket() {}

UDPSocket::UDPSocket(Port port): _Base(port) {}

UDPSocket::UDPSocket(IPv4 address, Port port): _Base(address, port) {}

UDPSocket::~UDPSocket() {
	_Base::~UDPSocket();
}

std::future<bool> UDPSocket::sendTo(const Host& host, Port port, const char* data, int length) {
	return std::async(
		std::launch::async,
		[this](const net::Host& h, Port p, const char* d, int l) -> bool {
			std::lock_guard lock(_sendMutex);
			return _Base::sendTo(h, p, d, l);
		},
		host.sync(),
		port,
		data,
		length
	);
}

std::future<bool> UDPSocket::sendTo(const Host& host, const char* data, int length) {
	return sendTo(host, _port, data, length);
}

std::future<bool> UDPSocket::sendTo(const Host& host, Port port, const std::string& data) {
	return sendTo(host, port, data.data(), data.length());
}

std::future<bool> UDPSocket::sendTo(const Host& host, const std::string& data) {
	return sendTo(host, _port, data);
}

std::future<bool> UDPSocket::recv(char* buf, int buflen, int& length, TimeoutMs timeout, bool peek) {
	return std::async(
		std::launch::async,
		[this](char* b, int bl, std::reference_wrapper<int> l, TimeoutMs t, bool p) -> bool {
			std::unique_lock lock(_recvMutex, std::defer_lock);
			std::chrono::milliseconds timeLeft;
			if (t < 0) { // timeout == inf
				lock.lock();
				timeLeft = std::chrono::milliseconds(t);
			} else { // timeout != inf
				auto timer = std::chrono::high_resolution_clock::now();
				if (lock.try_lock_for(std::chrono::milliseconds(t))) {
					timeLeft = std::chrono::milliseconds(t) -
						std::chrono::duration_cast<std::chrono::milliseconds>(
								   std::chrono::high_resolution_clock::now() - timer
						);
				} else {
					return false;
				}
			}

			pollfd pollData{};
			pollData.fd = _socket;
			pollData.events = POLLRDNORM;
			pollData.revents = 0;
			int result = poll(&pollData, 1, timeLeft.count());

			if (result == SOCKET_ERROR) {
				throw NetException(gai_strerror(netErrno()));
			}
			if (result == 0) { // timeout expired
				return false;
			}
			if (pollData.revents & POLLRDNORM) { // read
				return _Base::recv(b, bl, l.get(), p);
			}
			// cannot read
			return false;
		},
		buf,
		buflen,
		std::ref(length),
		timeout,
		peek
	);
}

std::future<bool> UDPSocket::recv(char* buf, int buflen, int& length, bool peek) {
	return recv(buf, buflen, length, -1, peek);
}

std::future<bool> UDPSocket::recv(char* buf, int buflen, TimeoutMs timeout, bool peek) {
	static int ignored;
	return recv(buf, buflen, ignored, timeout, peek);
}

std::future<bool> UDPSocket::recv(char* buf, int buflen, bool peek) {
	return recv(buf, buflen, -1, peek);
}

std::future<Host> UDPSocket::recvFrom(char* buf, int buflen, int& length, TimeoutMs timeout, bool peek) {
	return std::async(
		std::launch::async,
		[this](char* b, int bl, std::reference_wrapper<int> l, TimeoutMs t, bool p) -> Host {
			std::unique_lock lock(_recvMutex, std::defer_lock);
			std::chrono::milliseconds timeLeft;
			if (t < 0) { // timeout == inf
				lock.lock();
				timeLeft = std::chrono::milliseconds(t);
			} else { // timeout != inf
				auto timer = std::chrono::high_resolution_clock::now();
				if (lock.try_lock_for(std::chrono::milliseconds(t))) {
					timeLeft = std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::high_resolution_clock::now() - timer
					);
				} else {
					return Host(IPv4());
				}
			}

			pollfd pollData{};
			pollData.fd = _socket;
			pollData.events = POLLRDNORM;
			pollData.revents = 0;
			int result = poll(&pollData, 1, timeLeft.count());

			if (result == SOCKET_ERROR) {
				throw NetException(gai_strerror(netErrno()));
			}

			if (result == 0) { // timeout expired
				return Host(IPv4());
			}

			if (pollData.revents & POLLRDNORM) { // read
				return _Base::recvFrom(b, bl, l.get(), p).async();
			}

			// cannot read
			return Host(IPv4());
		},
		buf,
		buflen,
		std::ref(length),
		timeout,
		peek
	);
}

std::future<Host> UDPSocket::recvFrom(char* buf, int buflen, int& length, bool peek) {
	return recvFrom(buf, buflen, length, -1, peek);
}

std::future<Host> UDPSocket::recvFrom(char* buf, int buflen, TimeoutMs timeout, bool peek) {
	int ignored;
	return recvFrom(buf, buflen, ignored, timeout, peek);
}

std::future<Host> UDPSocket::recvFrom(char* buf, int buflen, bool peek) {
	return recvFrom(buf, buflen, -1, peek);
}

} // namespace arch::net::async
