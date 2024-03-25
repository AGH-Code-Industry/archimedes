#include <net/TCPSocket.h>
#include <net/IPv4.h>
#include <memory>
#include <net/Init.h>
#include <net/Exception.h>

namespace arch::net {
TCPSocket::TCPSocket() :
	Socket(Socket::Protocol::TCP) {}
TCPSocket::TCPSocket(Port port) :
	Socket(Socket::Protocol::TCP, port) {}
TCPSocket::TCPSocket(IPv4 address, Port port) :
	Socket(Socket::Protocol::TCP, address, port) {}
TCPSocket::~TCPSocket() {
	Socket::~Socket();
}

TCPSocket::LingerData TCPSocket::linger() const {
	::linger optval;
	socklen_t optlen = sizeof(optval);

	int result = getsockopt(_socket, SOL_SOCKET, SO_LINGER, (char*)&optval, &optlen);
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}

	return {(bool)optval.l_onoff, optval.l_linger};
}
void TCPSocket::linger(LingerData data) {
	::linger optval{data.linger, data.seconds};

	int result = setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}
}

bool TCPSocket::connect(const Host& host, Port port) {
	if (listening()) {
		throw NetException("socket marked as listening cannot be connected");
	}

	sockaddr_in sock;
	memset(&sock, 0, sizeof(sock));
	sock.sin_addr = host.ip();
	sock.sin_port = htons(port);
	sock.sin_family = AF_INET;

	int result = ::connect(_socket, (sockaddr*)&sock, sizeof(sock));
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}

	_peerAddr = host.ip();
	_status = 1;
	return true;
}
bool TCPSocket::condConnect(const Host& host, Port port, void* data, int dataLen, int responseLen, AcceptResponseHandler handler, void* handlerData) {
	connect(host, port);

	_status = 0;
	// send connection data
	int result = ::send(_socket, (char*)data, dataLen, 0);
	if (result == SOCKET_ERROR) {
		_peerAddr = IPv4();
		throw NetException(std::string("while sending connection data: ") + gai_strerror(net_errno()));
	}
	auto response_buffer = std::unique_ptr<char[]>(new char[responseLen]);
	memset(response_buffer.get(), 0, responseLen);
	// receive response
	result = ::recv(_socket, response_buffer.get(), responseLen, 0);
	if (result == SOCKET_ERROR) {
		_peerAddr = IPv4();
		throw NetException(std::string("while receiving response: ") + gai_strerror(net_errno()));
	}
	else if (result == 0) {
		throw NetException(std::string("peer disconnected before sending response"));
	}
	// handle response
	if (handler(response_buffer.get(), responseLen, handlerData)) {
		_status = 1;
		return true;
	}
	else {
		_peerAddr = IPv4();
		throw NetException("peer rejected connection");
	}
}
bool TCPSocket::connected() const {
	return _status & 1;
}
bool TCPSocket::connectedForce() {
	if (listening()) {
		return false;
	}

	pollfd pfd;
	pfd.fd = _socket;
	pfd.events = POLLRDNORM;
	pfd.revents = 0;
	int result = poll(&pfd, 1, 0);
	if (result != 1) {
		_status = 0;
		return false;
	}

	if (pfd.revents & POLLRDNORM) {
		char buf;
		result = ::recv(_socket, &buf, 0, MSG_PEEK);
		if (result == 0) {
			_status = 0;
			return false;
		}
		else {
			_status = 1;
			return true;
		}
	}
	else {
		return _status & 1;
	}
}

bool TCPSocket::listen() {
	return listen(-1);
}
bool TCPSocket::listen(int maxconn) {
	if (listening()) {
		return false;
	}
	if (connectedForce()) {
		return false;
	}
	if (maxconn == -1) {
		maxconn = SOMAXCONN;
	}

	int result = ::listen(_socket, maxconn);
	if (result != 0) {
		throw NetException(gai_strerror(net_errno()));
	}

	_status = 1 << 1;
	return true;
}
bool TCPSocket::listening() const {
	return _status & (1 << 1);
}

bool TCPSocket::accept(TCPSocket& newSock) {
	if (not listening()) {
		throw NetException("only listening sockets can accept()");
	}

	sockaddr_in sock;
	socklen_t socklen = sizeof(sock);
	memset(&sock, 0, sizeof(sock));
	auto result = ::accept(_socket, (sockaddr*)&sock, &socklen);
	if (result == INVALID_SOCKET) {
		throw NetException(gai_strerror(net_errno()));
	}

	newSock._address = _address;
	newSock._peerAddr = IPv4(sock.sin_addr);
	newSock._port = ntohs(sock.sin_port);
	newSock._proto = Socket::Protocol::TCP;
	newSock._socket = result;
	newSock._status = 1;

	return true;
}
bool TCPSocket::condAccept(TCPSocket& newSock, AcceptCondition condition, int dataLen, int responseLen, void* additionalData) {
	accept(newSock);

	newSock._status = 0;
	auto buffer = std::unique_ptr<char[]>(new char[dataLen]);
	memset(buffer.get(), 0, dataLen);
	// receive connection data
	int result = ::recv(newSock._socket, buffer.get(), dataLen, 0);
	if (result == SOCKET_ERROR) {
		throw NetException(std::string("while receiving connection data: ") + gai_strerror(net_errno()));
	}
	else if (result == 0) {
		throw NetException(std::string("peer disconnected before sending connection data"));
	}
	auto resp_buffer = std::unique_ptr<char[]>(new char[responseLen]);
	memset(resp_buffer.get(), 0, responseLen);
	auto return_result = condition(buffer.get(), dataLen, additionalData, resp_buffer.get(), responseLen);
	result = ::send(newSock._socket, resp_buffer.get(), responseLen, 0);
	if (result == SOCKET_ERROR) {
		throw NetException(std::string("while sending connection data: ") + gai_strerror(net_errno()));
	}
	if (return_result) {
		newSock._status = 1;
		return true;
	}

	newSock.close();
	throw NetException("rejected incoming connection");
}

bool TCPSocket::send(const char* data, int len) {
	if (not connected()) {
		return false;
	}

	int result = ::send(_socket, data, len, 0);
	if (result == SOCKET_ERROR) {
		throw NetException(gai_strerror(net_errno()));
	}

	return true;
}
bool TCPSocket::send(const std::string& data) {
	return send(data.data(), data.length());
}

bool TCPSocket::recv(char* buf, int buflen, int& length, bool peek) {
	if (not connected()) {
		return false;
	}

	int result = ::recv(_socket, buf, buflen, 0);
	if (result == SOCKET_ERROR) {
		throw NetException(gai_strerror(net_errno()));
	}
	else if (result == 0) {
		_status = 0;
		throw NetException(std::string("peer disconnected"));
	}

	length = result;
	return true;
}
bool TCPSocket::recv(char* buf, int buflen, bool peek) {
	static int ignored;
	return recv(buf, buflen, ignored, peek);
}
}