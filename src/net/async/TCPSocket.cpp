#include "net/async/TCPSocket.h"

#include <net/NetException.h>

namespace arch::net::async {

TCPSocket::TCPSocket() {}

TCPSocket::TCPSocket(Port port): _Base(port) {}

TCPSocket::TCPSocket(IPv4 address, Port port): _Base(address, port) {}

TCPSocket::~TCPSocket() {
	Socket::~Socket();
}

std::future<bool> TCPSocket::connectedForce() {
	return std::async(std::launch::async, [this]() -> bool { return _Base::connectedForce(); });
}

std::future<bool> TCPSocket::connect(const Host& host, Port port) {
	return std::async(
		std::launch::async,
		[this](const Host& h, Port p) -> bool { return _Base::connect(h.sync(), p); },
		host,
		port
	);
}

std::future<bool> TCPSocket::condConnect(
	const Host& host,
	Port port,
	void* data,
	int dataLen,
	int responseLen,
	AcceptResponseHandler handler,
	void* handlerData
) {
	return std::async(
		std::launch::async,
		[this](const Host& h, Port p, void* d, int dl, int rl, AcceptResponseHandler h2, void* hd = nullptr) -> bool {
			std::scoped_lock lock(_sendMutex, _recvMutex);
			return _Base::condConnect(h.sync(), p, d, dl, rl, h2, hd);
		},
		host,
		port,
		data,
		dataLen,
		responseLen,
		handler,
		handlerData
	);
}

std::future<bool> TCPSocket::listen() {
	return std::async(std::launch::async, [this]() -> bool { return _Base::listen(); });
}

std::future<bool> TCPSocket::listen(int maxconn) {
	return std::async(std::launch::async, [this](int m) -> bool { return _Base::listen(m); }, maxconn);
}

std::future<bool> TCPSocket::accept(TCPSocket& newSock) const {
	return std::async(
		std::launch::async,
		[this](std::reference_wrapper<TCPSocket> ns) -> bool {
			TCPSocket& socket = ns.get();

			if (not listening()) {
				throw NetException("only listening sockets can accept()");
			}

			sockaddr_in sock;
			socklen_t socklen = sizeof(sock);
			memset(&sock, 0, sizeof(sock));
			auto result = ::accept(_socket, (sockaddr*)&sock, &socklen);
			if (result == INVALID_SOCKET) {
				throw NetException(gai_strerror(netErrno()));
			}

			socket._address = _address;
			socket._peerAddr = IPv4(sock.sin_addr);
			socket._port = ntohs(sock.sin_port);
			socket._proto = Socket::Protocol::tcp;
			socket._socket = result;
			socket._status = 1;

			return true;
		},
		std::ref(newSock)
	);
}

std::future<bool> TCPSocket::condAccept(
	TCPSocket& newSock,
	AcceptCondition condition,
	int dataLen,
	int responseLen,
	void* additionalData
) {
	return std::async(
		std::launch::async,
		[this](std::reference_wrapper<TCPSocket> ns, AcceptCondition c, int dl, int rl, void* ad) -> bool {
			std::scoped_lock lock(_sendMutex, _recvMutex);
			TCPSocket& socket = ns.get();

			auto acceptFuture = accept(socket);

			socket._status = 0;
			auto buffer = std::unique_ptr<char[]>(new char[dl]);
			memset(buffer.get(), 0, dl);
			// receive connection data
			int result = ::recv(socket._socket, buffer.get(), dl, 0);
			if (result == SOCKET_ERROR) {
				throw NetException(std::string("while receiving connection data: ") + gai_strerror(netErrno()));
			}
			if (result == 0) {
				throw NetException(std::string("peer disconnected before sending connection data"));
			}
			auto respBuffer = std::unique_ptr<char[]>(new char[rl]);
			memset(respBuffer.get(), 0, rl);
			auto returnResult = c(buffer.get(), dl, ad, respBuffer.get(), rl);
			result = ::send(socket._socket, respBuffer.get(), rl, 0);
			if (result == SOCKET_ERROR) {
				throw NetException(std::string("while sending connection data: ") + gai_strerror(netErrno()));
			}
			if (returnResult) {
				socket._status = 1;
				return true;
			}

			return false;
		},
		std::ref(newSock),
		condition,
		dataLen,
		responseLen,
		additionalData
	);
}

std::future<bool> TCPSocket::send(const char* data, int len) {
	std::lock_guard lock(_sendMutex);
	return std::async(
		std::launch::async,
		[this](const char* d, int l) -> bool {
			std::lock_guard lock(_sendMutex);
			return _Base::send(d, l);
		},
		data,
		len
	);
}

std::future<bool> TCPSocket::send(const std::string& data) {
	return std::async(
		std::launch::async,
		[this](const std::string& d) -> bool {
			std::lock_guard lock(_sendMutex);
			return _Base::send(d);
		},
		data
	);
}

std::future<bool> TCPSocket::recv(char* buf, int buflen, int& length, TimeoutMs timeout, bool peek) {
	return std::async(
		std::launch::async,
		[this](char* b, int bl, std::reference_wrapper<int> l, TimeoutMs t, bool p) -> bool {
			std::chrono::milliseconds timeLeft;
			if (t < 0) { // timeout == inf
				_recvMutex.lock();
				timeLeft = std::chrono::milliseconds(t);
			} else { // timeout != inf
				auto timer = std::chrono::high_resolution_clock::now();
				if (_recvMutex.try_lock_for(std::chrono::milliseconds(t))) {
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
				_recvMutex.unlock();
				return false;
			}
			if (pollData.revents & POLLRDNORM) { // read
				try {
					auto res = _Base::recv(b, bl, l.get(), p);
					_recvMutex.unlock();
					return res;
				} catch (NetException e) {
					_recvMutex.unlock();
					throw;
				}
			}
			_recvMutex.unlock();
			return false;
		},
		buf,
		buflen,
		std::ref(length),
		timeout,
		peek
	);
}

std::future<bool> TCPSocket::recv(char* buf, int buflen, TimeoutMs timeout, bool peek) {
	static int ignored;
	return recv(buf, buflen, ignored, timeout, peek);
}

std::future<bool> TCPSocket::recv(char* buf, int buflen, int& length, bool peek) {
	return recv(buf, buflen, length, -1, peek);
}

std::future<bool> TCPSocket::recv(char* buf, int buflen, bool peek) {
	static int ignored;
	return recv(buf, buflen, ignored, peek);
}

} // namespace arch::net::async
