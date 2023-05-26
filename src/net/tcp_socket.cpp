#include <net/tcp_socket.hpp>

namespace arch::net {
	constexpr bool TCPSocket::no_condition(void*, int, void*) {
		return true;
	}

	TCPSocket::TCPSocket() :
		Socket(Socket::tcp) {}
	TCPSocket::TCPSocket(port_type port) :
		Socket(Socket::tcp, port) {}
	TCPSocket::TCPSocket(IPv4 address, port_type port) :
		Socket(Socket::tcp, address, port) {}
	/*
	TCPSocket::TCPSocket(sock_type s, port_type p, IPv4 addr, IPv4 paddr) {
		this->_address = addr;
		this->_connected = true;
		this->_peer_addr = paddr;
		this->_proto = tcp;
		this->_port = p;
		this->_socket = s;
	}
	*/

	bool TCPSocket::connect(const Host& host, port_type port) {
		if (listening()) {
			// log error
			return false;
		}

		sockaddr_in sock;
		memset(&sock, 0, sizeof(sock));
		sock.sin_addr = host.ip();
		sock.sin_port = htons(port);
		sock.sin_family = AF_INET;

		int result = ::connect(_socket, (sockaddr*)&sock, sizeof(sock));
		if (result != 0) {
			// log error
			return false;
		}

		_peer_addr = host.ip();
		_status = 1;
		return true;
	}
	bool TCPSocket::connected() const {
		return _status & 1;
	}

	bool TCPSocket::listen() {
		return listen(-1);
	}
	bool TCPSocket::listen(int maxconn) {
		if (connected()) {
			// log error
			return false;
		}
		if (maxconn == -1) {
			maxconn = SOMAXCONN;
		}

		int result = ::listen(_socket, maxconn);
		if (result != 0) {
			// log error
			return false;
		}

		_status = 1 << 1;
		return true;
	}
	bool TCPSocket::listening() const {
		return _status & (1 << 1);
	}

	bool TCPSocket::accept(TCPSocket& new_sock) {
		if (not listening()) {
			// log error
			return false;
		}

		sockaddr_in sock;
		socklen_t socklen = sizeof(sock);
		memset(&sock, 0, sizeof(sock));
		auto result = ::accept(_socket, (sockaddr*)&sock, &socklen);
		if (result == INVALID_SOCKET) {
			// log error
			return false;
		}

		new_sock._address = _address;
		new_sock._peer_addr = IPv4(sock.sin_addr);
		new_sock._port = ntohs(sock.sin_port);
		new_sock._proto = tcp;
		new_sock._socket = result;
		new_sock._status = 1;

		return true;
	}
	
	bool TCPSocket::send(const char* data, int len) {
		int result = ::send(_socket, data, len, 0);
		if (result == SOCKET_ERROR) {
			// log error
			return false;
		}

		return true;
	}
	bool TCPSocket::send(std::string_view data) {
		return send(data.data(), data.length());
	}

	bool TCPSocket::recv(char* buf, int buflen, int& length, bool peek) {
		int result = ::recv(_socket, buf, buflen, 0);
		if (result == SOCKET_ERROR) {
			// log error
			return false;
		}

		length = result;
		return true;
	}
	bool TCPSocket::recv(char* buf, int buflen, bool peek) {
		int temp;
		return recv(buf, buflen, temp, peek);
	}
}