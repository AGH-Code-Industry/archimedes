#include <net/socket.hpp>

namespace arch::net {
	const IPv4 Socket::any_address(INADDR_ANY);
	const Socket::port_type Socket::random_port = 0;

	Socket::Socket(sock_protocol p) {
		switch (p) {
			case UDP:
				_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				break;
			case TCP:
				_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				break;
			default:
				// log error
				break;
		}

		if (_socket == INVALID_SOCKET) {
			// log error
		}
		else if (_socket == NULL) {
			// log error
		}

		_proto = p;
	}
	Socket::Socket(sock_protocol p, IPv4 address, port_type port) :
		Socket(p) {
		if (not bind(address, port)) {
			// log error
		}
	}
	Socket::Socket(sock_protocol p, port_type port) :
		Socket(p) {
		if(not bind(port)) {
			// log error
		}
	}
	Socket::~Socket() {
		close();
	}

	void Socket::close() {
		if (_socket != NULL) {
			closesocket(_socket);
		}
	}
	bool Socket::bind(IPv4 address, port_type port) {
		int result;
		sockaddr_in sock;
		memset(&sock, 0, sizeof(sock));
		sock.sin_addr = address;
		sock.sin_family = AF_INET;
		sock.sin_port = htons(port);

		result = ::bind(_socket, (sockaddr*)&sock, sizeof(sock));

		if (result != 0) {
			// log error
			return 0;
		}

		_address = address;
		_port = port;

		return 1;
	}
	bool Socket::bind(port_type port) {
		return bind(any_address, port);
	}
	Socket::port_type Socket::bind() {
		if (bind(random_port)) {
			sockaddr_in data;
			memset(&data, 0, sizeof(data));
			int len = sizeof(data);
			getsockname(_socket, (sockaddr*)&data, &len);

			return ntohs(data.sin_port);
		}
		
		return 0;
	}

	IPv4 Socket::address() const {
		return _address;
	}
	Socket::port_type Socket::port() const {
		return _port;
	}
	Socket::sock_protocol Socket::protocol() const {
		return _proto;
	}
	bool Socket::bound() const {
		return _port != 0;
	}
}