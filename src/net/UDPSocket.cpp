#include <net/UDPSocket.h>
#include <net/Exception.h>
#include <net/IPv4.h>
#include <net/Init.h>

namespace arch::net {
UDPSocket::UDPSocket() :
	Socket(Socket::Protocol::UDP) {}
UDPSocket::UDPSocket(Port port) :
	Socket(Socket::Protocol::UDP, port) {}
UDPSocket::UDPSocket(IPv4 address, Port port) :
	Socket(Socket::Protocol::UDP, address, port) {}
UDPSocket::~UDPSocket() {
	Socket::~Socket();
}

bool UDPSocket::sendTo(const Host& host, Port port, const char* data, int len) {
	// address settings
	sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr = host.ip();
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	int result = sendto(_socket, data, len, 0, (sockaddr*)&addr, sizeof(addr));
	if (result == SOCKET_ERROR) {
		throw NetException(gai_strerror(net_errno()));
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
		throw NetException(gai_strerror(net_errno()));
	}

	length = result;
	return result;
}
bool UDPSocket::recv(char* buf, int buflen, bool peek) {
	int temp;
	return recv(buf, buflen, temp, peek);
}
Host UDPSocket::recvFrom(char* buf, int buflen, int& length, bool peek) {
	sockaddr_in addr;
	socklen_t addrLen;
	addrLen = sizeof(addr);
	memset(&addr, 0, sizeof(addr));

	int result = ::recvfrom(_socket, buf, buflen, peek ? MSG_PEEK : 0, (sockaddr*)&addr, &addrLen);
	if (result == SOCKET_ERROR) {
		throw NetException(gai_strerror(net_errno()));
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
		throw NetException(gai_strerror(net_errno(result)));
	}

	return optval;
}
void UDPSocket::broadcastEnabled(bool newVal) {
	int optval = newVal;

	int result = setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}
}
}