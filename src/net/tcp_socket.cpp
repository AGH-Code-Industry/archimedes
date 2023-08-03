#include <net/tcp_socket.hpp>
#include <memory>
#include <net/exception.hpp>

namespace arch::net {
TCPSocket::TCPSocket() :
	Socket(Socket::protocol_t::TCP) {}
TCPSocket::TCPSocket(port_type port) :
	Socket(Socket::protocol_t::TCP, port) {}
TCPSocket::TCPSocket(IPv4 address, port_type port) :
	Socket(Socket::protocol_t::TCP, address, port) {}
TCPSocket::~TCPSocket() {
	Socket::~Socket();
}

TCPSocket::linger_data TCPSocket::linger() const {
	::linger optval;
	socklen_t optlen = sizeof(optval);

	int result = getsockopt(_socket, SOL_SOCKET, SO_LINGER, (char*)&optval, &optlen);
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}

	return {(bool)optval.l_onoff, optval.l_linger};
}
void TCPSocket::linger(linger_data data) {
	::linger optval{data.linger, data.seconds};

	int result = setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
	if (result != 0) {
		throw NetException(gai_strerror(net_errno(result)));
	}
}

bool TCPSocket::connect(const Host& host, port_type port) {
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

	_peer_addr = host.ip();
	_status = 1;
	return true;
}
bool TCPSocket::cond_connect(const Host& host, port_type port, void* data, int data_len, int response_len, accept_response_handler handler, void* handler_data) {
	try {
		connect(host, port);
	}
	catch (NetException e) {
		_peer_addr = IPv4();
		throw e;
	}

	_status = 0;
	// send connection data
	int result = ::send(_socket, (char*)data, data_len, 0);
	if (result == SOCKET_ERROR) {
		_peer_addr = IPv4();
		throw NetException(std::string("while sending connection data: ") + gai_strerror(net_errno()));
	}
	auto response_buffer = std::unique_ptr<char[]>(new char[response_len]);
	memset(response_buffer.get(), 0, response_len);
	// receive response
	result = ::recv(_socket, response_buffer.get(), response_len, 0);
	if (result == SOCKET_ERROR) {
		_peer_addr = IPv4();
		throw NetException(std::string("while receiving response: ") + gai_strerror(net_errno()));
	}
	else if (result == 0) {
		throw NetException(std::string("peer disconnected before sending response"));
	}
	// handle response
	if (handler(response_buffer.get(), response_len, handler_data)) {
		_status = 1;
		return true;
	}
	else {
		_peer_addr = IPv4();
		throw NetException("peer rejected connection");
	}
}
bool TCPSocket::connected() const {
	return _status & 1;
}
bool TCPSocket::connected_force() {
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
	try {
		if (connected_force()) {
			return false;
		}
	}
	catch (NetException e) {
		throw e;
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

bool TCPSocket::accept(TCPSocket& new_sock) {
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

	new_sock._address = _address;
	new_sock._peer_addr = IPv4(sock.sin_addr);
	new_sock._port = ntohs(sock.sin_port);
	new_sock._proto = Socket::protocol_t::TCP;
	new_sock._socket = result;
	new_sock._status = 1;

	return true;
}
bool TCPSocket::cond_accept(TCPSocket& new_sock, accept_condition condition, int data_len, int response_len, void* additional_data) {
	try {
		accept(new_sock);
	}
	catch (NetException e) {
		throw e;
	}

	new_sock._status = 0;
	auto buffer = std::unique_ptr<char[]>(new char[data_len]);
	memset(buffer.get(), 0, data_len);
	// receive connection data
	int result = ::recv(new_sock._socket, buffer.get(), data_len, 0);
	if (result == SOCKET_ERROR) {
		throw NetException(std::string("while receiving connection data: ") + gai_strerror(net_errno()));
	}
	else if (result == 0) {
		throw NetException(std::string("peer disconnected before sending connection data"));
	}
	auto resp_buffer = std::unique_ptr<char[]>(new char[response_len]);
	memset(resp_buffer.get(), 0, response_len);
	auto return_result = condition(buffer.get(), data_len, additional_data, resp_buffer.get(), response_len);
	result = ::send(new_sock._socket, resp_buffer.get(), response_len, 0);
	if (result == SOCKET_ERROR) {
		throw NetException(std::string("while sending connection data: ") + gai_strerror(net_errno()));
	}
	if (return_result) {
		new_sock._status = 1;
		return true;
	}

	try {
		new_sock.close();
	}
	catch (NetException e) {
		throw e;
	}
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
bool TCPSocket::send(std::string_view data) {
	try {
		return send(data.data(), data.length());
	}
	catch (NetException e) {
		throw e;
	}
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
	int temp;
	try {
		return recv(buf, buflen, temp, peek);
	}
	catch (NetException e) {
		throw e;
	}
}
}