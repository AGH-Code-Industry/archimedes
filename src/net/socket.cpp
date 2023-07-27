#include <net/socket.hpp>

namespace arch::net {
	const IPv4 Socket::any_address(INADDR_ANY);
	const Socket::port_type Socket::random_port = 0;

	Socket::Socket(protocol_t p) {
		switch (p) {
			case protocol_t::UDP:
				_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				break;
			case protocol_t::TCP:
				_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				break;
			default:
				// log error
				break;
		}

		if (_socket == INVALID_SOCKET) {
			// log error
		}

		_proto = p;
	}
	Socket::Socket(protocol_t p, IPv4 address, port_type port) :
		Socket(p) {
		if (not bind(address, port)) {
			// log error
		}
	}
	Socket::Socket(protocol_t p, port_type port) :
		Socket(p) {
		if (not bind(port)) {
			// log error
		}
	}
	Socket::~Socket() {
		close();
	}

	void Socket::close() {
		if (_socket != INVALID_SOCKET) {
			if (
#ifdef _WIN32
				closesocket(_socket)
#elif defined unix
				::close(_socket)
#endif
				!= 0) {
				// log error
			}

			_socket = INVALID_SOCKET;
			_address.data(0);
			_port = 0;
			_proto = Socket::protocol_t::None;
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
			close();
			return false;
		}

		_address = address;
		_port = port;

		return true;
	}
	bool Socket::bind(port_type port) {
		return bind(any_address, port);
	}
	Socket::port_type Socket::bind() {
		if (bind(random_port)) {
			sockaddr_in data;
			memset(&data, 0, sizeof(data));
			socklen_t len = sizeof(data);
			getsockname(_socket, (sockaddr*)&data, &len);

			_port = (data.sin_port);
			return _port;
		}

		return 0;
	}

	IPv4 Socket::address() const {
		return _address;
	}
	Socket::port_type Socket::port() const {
		return _port;
	}
	Socket::protocol_t Socket::protocol() const {
		return _proto;
	}
	bool Socket::bound() const {
		return _port != 0;
	}
	bool Socket::data_avalible() const {
		pollfd poll_data;
		memset(&poll_data, 0, sizeof(poll_data));
		poll_data.fd = _socket;
		poll_data.events = POLLRDNORM;

		bool retval = true;

		int result = poll(&poll_data, 1, 0);
		if (result != 1) {
			// log error
			return false;
		}
		if (poll_data.revents & POLLERR) {
			// log error
			retval = false;
		}
		if (poll_data.revents & POLLNVAL) {
			// log error
			retval = false;
		}
		if (not (poll_data.revents & POLLRDNORM)) {
			retval = false;
		}

		return retval;
	}
	bool Socket::sendable() const {
		pollfd poll_data;
		memset(&poll_data, 0, sizeof(poll_data));
		poll_data.fd = _socket;

		bool retval = true;

		int result = poll(&poll_data, 1, 0);
		if (result != 1) {
			// log error
			return false;
		}
		if (poll_data.revents & POLLERR) {
			// log error
			retval = false;
		}
		if (poll_data.revents & POLLNVAL) {
			// log error
			retval = false;
		}
		if (not (poll_data.revents & POLLWRNORM)) {
			retval = false;
		}

		return retval;
	}
	Socket::usable_data Socket::usable() const {
		pollfd poll_data;
		memset(&poll_data, 0, sizeof(poll_data));
		poll_data.fd = _socket;

		bool retval = true;

		int result = poll(&poll_data, 1, 0);
		if (result != 1) {
			// log error
			return {false, false};
		}
		if (poll_data.revents & POLLERR) {
			// log error
			retval = false;
		}
		if (poll_data.revents & POLLNVAL) {
			// log error
			retval = false;
		}
		if (not retval) {
			return {false, false};
		}

		usable_data data;
		data.data_avalible = bool(poll_data.revents & POLLRDNORM);
		data.sendable = bool(poll_data.revents & POLLWRNORM);
		return data;
	}

	int Socket::recv_buf() const {
		int optval;
		socklen_t optlen = sizeof(optval);

		int result = getsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&optval, &optlen);
		if (result != 0) {
			// log error
			return 0;
		}

		return optval;
	}
	void Socket::recv_buf(int new_val) {
		int optval = new_val;

		int result = setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (char*)&optval, sizeof(optval));
		if (result != 0) {
			// log error;
		}
	}
	int Socket::send_buf() const {
		int optval;
		socklen_t optlen = sizeof(optval);

		int result = getsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char*)&optval, &optlen);
		if (result != 0) {
			// log error
			return 0;
		}

		return optval;
	}
	void Socket::send_buf(int new_val) {
		int optval = new_val;

		int result = setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (char*)&optval, sizeof(optval));
		if (result != 0) {
			// log error;
		}
	}
#ifdef _WIN32 // exclusivity avalible only on Windows
	bool Socket::exclusive() const {
		int optval;
		socklen_t optlen = sizeof(optval);

		int result = getsockopt(_socket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, &optlen);
		if (result != 0) {
			// log error
			return 0;
		}

		return optval;
	}
	void Socket::exclusive(bool new_val) {
		int optval = new_val;

		int result = setsockopt(_socket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&optval, sizeof(optval));
		if (result != 0) {
			// log error;
		}
	}
#endif
	bool Socket::reuse() const {
		int optval;
		socklen_t optlen = sizeof(optval);

		int result = getsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, &optlen);
		if (result != 0) {
			// log error
			return 0;
		}

		return optval;
	}
	void Socket::reuse(bool new_val) {
		int optval = new_val;

		int result = setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
		if (result != 0) {
			// log error;
		}
#ifdef unix
		optval = new_val;
		int result2 = setsockopt(_socket, SOL_SOCKET, SO_REUSEPORT, (char*)&optval, sizeof(optval));
		if (result2 != 0) {
			// log error;
		}
#endif
	}
}