#include <net/tcp_socket.hpp>
#include <memory>

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
	/*
	TCPSocket::TCPSocket(sock_type s, port_type p, IPv4 addr, IPv4 paddr) {
		this->_address = addr;
		this->_connected = true;
		this->_peer_addr = paddr;
		this->_proto = TCP;
		this->_port = p;
		this->_socket = s;
	}
	*/

	TCPSocket::linger_data TCPSocket::linger() const {
		::linger optval;
		socklen_t optlen = sizeof(optval);

		int result = getsockopt(_socket, SOL_SOCKET, SO_LINGER, (char*)&optval, &optlen);
		if (result != 0) {
			// log error
			return {0, 0};
		}

		return {(bool)optval.l_onoff, optval.l_linger};
	}
	void TCPSocket::linger(linger_data data) {
		::linger optval{data.linger, data.seconds};

		int result = setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
		if (result != 0) {
			// log error;
		}
	}

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
	bool TCPSocket::cond_connect(const Host& host, port_type port, void* data, int data_len, int response_len, accept_response_handler handler, void* handler_data) {
		if (not connect(host, port)) {
			// log error
			_peer_addr = IPv4();
			return false;
		}

		_status = 0;
		// send connection data
		int result = ::send(_socket, (char*)data, data_len, 0);
		if (result == SOCKET_ERROR) {
			// log error
			_peer_addr = IPv4();
			return false;
		}
		auto response_buffer = std::unique_ptr<char[]>(new char[response_len]);
		memset(response_buffer.get(), 0, response_len);
		// receive response
		result = ::recv(_socket, response_buffer.get(), response_len, 0);
		if (result == SOCKET_ERROR) {
			// log error
			_peer_addr = IPv4();
			return false;
		}
		// handle response
		if (handler(response_buffer.get(), response_len, handler_data)) {
			_status = 1;
			return true;
		}
		else {
			// log error
			_peer_addr = IPv4();
			return false;
		}
	}
	bool TCPSocket::connected() const {
		return _status & 1;
	}
	bool TCPSocket::connected_force() {
		if (listening()) {
			// log error
			return false;
		}

		pollfd pfd;
		pfd.fd = _socket;
		pfd.events = POLLIN;
		pfd.revents = 0;
		int result = poll(&pfd, 1, 0);
		if (result != 1) {
			// log error
			_status = 0;
			return false;
		}
		
		if (pfd.revents & POLLIN) {
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
		new_sock._proto = Socket::protocol_t::TCP;
		new_sock._socket = result;
		new_sock._status = 1;

		return true;
	}
	bool TCPSocket::cond_accept(TCPSocket& new_sock, accept_condition condition, int data_len, int response_len, void* additional_data) {
		if (not accept(new_sock)) {
			// log error
			return false;
		}

		new_sock._status = 0;
		auto buffer = std::unique_ptr<char[]>(new char[data_len]);
		memset(buffer.get(), 0, data_len);
		// receive connection data
		int result = ::recv(new_sock._socket, buffer.get(), data_len, 0);
		if (result == SOCKET_ERROR) {
			// log error
			return false;
		}
		auto resp_buffer = std::unique_ptr<char[]>(new char[response_len]);
		memset(resp_buffer.get(), 0, response_len);
		auto return_result = condition(buffer.get(), data_len, additional_data, resp_buffer.get(), response_len);
		result = ::send(new_sock._socket, resp_buffer.get(), response_len, 0);
		if (result == SOCKET_ERROR) {
			// log error
			return false;
		}
		if(return_result){
			new_sock._status = 1;
			return true;
		}

		return false;
	}
	
	bool TCPSocket::send(const char* data, int len) {
		if (not connected()) {
			return false;
		}

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
		if (not connected()) {
			return false;
		}

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