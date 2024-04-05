#include "net/UDPSocket.h"

#include "net/IPv4.h"
#include "net/NetException.h"

namespace arch::net {

UDPSocket::UDPSocket(): Socket(Protocol::udp) {}

UDPSocket::UDPSocket(Port port): Socket(Protocol::udp, port) {}

UDPSocket::UDPSocket(IPv4 address, Port port): Socket(Protocol::udp, address, port) {}

UDPSocket::~UDPSocket() {
	Socket::~Socket();
}

bool UDPSocket::sendTo(const Host& host, Port port, const char* data, int len) {
	// address settings
	sockaddr_in addr = {};
	addr.sin_addr = host.ip();
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	int result = sendto(_socket, data, len, 0, (sockaddr*)&addr, sizeof(addr));
	if (result == SOCKET_ERROR) {
		throw NetException(gai_strerror(netErrno()));
	}

	return result;
}

bool UDPSocket::sendTo(const Host& host, const char* data, int len) {
	return sendTo(host, _port, data, len);
}

bool UDPSocket::sendTo(const Host& host, Port port, const std::string& data) {
	return sendTo(host, port, data.data(), data.length());
}

bool UDPSocket::sendTo(const Host& host, const std::string& data) {
	return sendTo(host, _port, data);
}

bool UDPSocket::recv(char* buf, int buflen, int& length, bool peek) {
	int result = ::recv(_socket, buf, buflen, peek ? MSG_PEEK : 0);
	if (result == SOCKET_ERROR) {
		throw NetException(gai_strerror(netErrno()));
	}

	length = result;
	return result;
}

bool UDPSocket::recv(char* buf, int buflen, bool peek) {
	int temp;
	return recv(buf, buflen, temp, peek);
}

Host UDPSocket::recvFrom(char* buf, int buflen, int& length, bool peek) {
	sockaddr_in addr = {};
	socklen_t addrLen;
	addrLen = sizeof(addr);

	int result = ::recvfrom(_socket, buf, buflen, peek ? MSG_PEEK : 0, (sockaddr*)&addr, &addrLen);
	if (result == SOCKET_ERROR) {
		throw NetException(gai_strerror(netErrno()));
	}

	length = result;
	return Host(IPv4(addr.sin_addr));
}

Host UDPSocket::recvFrom(char* buf, int buflen, bool peek) {
	static int ignored;
	return recvFrom(buf, buflen, ignored, peek);
}

bool UDPSocket::broadcastEnabled() const {
	int optval;
	socklen_t optlen = sizeof(optval);

	int result = getsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, &optlen);
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}

	return optval;
}

void UDPSocket::broadcastEnabled(bool newVal) {
	int optval = newVal;

	int result = setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(netErrno(result)));
	}
}

} // namespace arch::net
