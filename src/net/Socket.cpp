#include "net/Socket.h"

#include "net/IPv4.h"
#include "net/NetException.h"

namespace arch::net {

const IPv4 Socket::anyAddress((uint32_t)INADDR_ANY);
const Socket::Port Socket::randomPort = 0;

Socket::Socket(Protocol protocol) {
	switch (protocol) {
		case Protocol::udp: _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); break;
		case Protocol::tcp: _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); break;
		default:			throw std::invalid_argument("invalid protocol value");
	}

	if (_socket == INVALID_SOCKET) {
		throw NetException(gai_strerror(netErrno()));
	}

	_proto = protocol;
}

Socket::Socket(Protocol protocol, IPv4 address, Port port): Socket(protocol) {
	bind(address, port);
}

Socket::Socket(Protocol protocol, Port port): Socket(protocol) {
	bind(port);
}

Socket::~Socket() {
	close();
}

void Socket::close() {
	if (_socket != INVALID_SOCKET) {
		int result = closesocket(_socket);
		if (result != 0) {
			throw NetException(gai_strerror(netErrno(result)));
		}

		_socket = INVALID_SOCKET;
		_address.data(0);
		_port = 0;
		_proto = Socket::Protocol::none;
	}
}

bool Socket::bind(IPv4 address, Port port) {
	// socket settings
	sockaddr_in sock = {};
	sock.sin_addr = address;
	sock.sin_family = AF_INET;
	sock.sin_port = htons(port);

	int result = ::bind(_socket, (sockaddr*)&sock, sizeof(sock));

	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}

	_address = address;
	_port = port;

	return true;
}

bool Socket::bind(Port port) {
	return bind(anyAddress, port);
}

Socket::Port Socket::bind() {
	bind(randomPort);

	sockaddr_in data = {};
	socklen_t len = sizeof(data);
	getsockname(_socket, (sockaddr*)&data, &len);

	_port = (data.sin_port);
	return _port;
}

IPv4 Socket::address() const {
	return _address;
}

Socket::Port Socket::port() const {
	return _port;
}

Socket::Protocol Socket::protocol() const {
	return _proto;
}

bool Socket::bound() const {
	return _port != 0;
}

bool Socket::dataAvalible() const {
	pollfd pollData = {};
	pollData.fd = _socket;
	pollData.events = POLLRDNORM;

	bool retval = true;

	int result = poll(&pollData, 1, 0);
	if (result != 1) {
		return false;
	}
	if (pollData.revents & POLLERR) {
		retval = false;
	}
	if (pollData.revents & POLLNVAL) {
		retval = false;
	}
	if (not(pollData.revents & POLLRDNORM)) {
		retval = false;
	}

	return retval;
}

bool Socket::sendable() const {
	pollfd pollData = {};
	pollData.fd = _socket;

	bool retval = true;

	int result = poll(&pollData, 1, 0);
	if (result != 1) {
		return false;
	}
	if (pollData.revents & POLLERR) {
		retval = false;
	}
	if (pollData.revents & POLLNVAL) {
		retval = false;
	}
	if (not(pollData.revents & POLLWRNORM)) {
		retval = false;
	}

	return retval;
}

Socket::UsableData Socket::usable() const {
	pollfd pollData = {};
	pollData.fd = _socket;

	bool retval = true;

	int result = poll(&pollData, 1, 0);
	if (result != 1) {
		return {false, false};
	}
	if (pollData.revents & POLLERR) {
		retval = false;
	}
	if (pollData.revents & POLLNVAL) {
		retval = false;
	}
	if (not retval) {
		return {false, false};
	}

	UsableData data;
	data.dataAvalible = (bool)(pollData.revents & POLLRDNORM);
	data.sendable = (bool)(pollData.revents & POLLWRNORM);
	return data;
}

int Socket::recvBuf() const {
	int optval;
	socklen_t optlen = sizeof(optval);

	int result = getsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&optval, &optlen);
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}

	return optval;
}

void Socket::recvBuf(int newVal) const {
	int optval = newVal;

	int result = setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}
}

int Socket::sendBuf() const {
	int optval;
	socklen_t optlen = sizeof(optval);

	int result = getsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char*)&optval, &optlen);
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}

	return optval;
}

void Socket::sendBuf(int newVal) const {
	int optval = newVal;

	int result = setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}
}
#if ARCHIMEDES_WINDOWS // exclusivity avalible only on Windows :(
bool Socket::exclusive() const {
	int optval;
	socklen_t optlen = sizeof(optval);

	int result = getsockopt(_socket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, &optlen);
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}

	return optval;
}

void Socket::exclusive(bool newVal) const {
	int optval = newVal;

	int result = setsockopt(_socket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}
}
#endif
bool Socket::reuse() const {
	int optval;
	socklen_t optlen = sizeof(optval);

	int result = getsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, &optlen);
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}

	return optval;
}

void Socket::reuse(bool newVal) const {
	int optval = newVal;

	int result = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}
#if ARCHIMEDES_UNIX
	optval = newVal;
	result = setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}
#endif
}

} // namespace arch::net
