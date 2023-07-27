#include <net/udp_socket.hpp>

namespace arch::net {
	UDPSocket::UDPSocket() :
		Socket(Socket::protocol_t::UDP) {}
	UDPSocket::UDPSocket(port_type port) :
		Socket(Socket::protocol_t::UDP, port) {}
	UDPSocket::UDPSocket(IPv4 address, port_type port) :
		Socket(Socket::protocol_t::UDP, address, port) {}
	UDPSocket::~UDPSocket() {
		Socket::~Socket();
	}

	bool UDPSocket::send_to(const Host& host, port_type port, const char* data, int len) {
		sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		addr.sin_addr = host.ip();
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

		int result = sendto(_socket, data, len, 0, (sockaddr*)&addr, sizeof(addr));
		if (result == SOCKET_ERROR) {
			// log error
			return false;
		}

		return result;
	}
	bool UDPSocket::send_to(const Host& host, const char* data, int len) {
		return send_to(host, _port, data, len);
	}
	bool UDPSocket::send_to(const Host& host, port_type port, std::string_view data) {
		return send_to(host, port, data.data(), data.length());
	}
	bool UDPSocket::send_to(const Host& host, std::string_view data) {
		return send_to(host, _port, data);
	}

	bool UDPSocket::recv(char* buf, int buflen, int& length, bool peek) {
		int result = ::recv(_socket, buf, buflen, peek ? MSG_PEEK : 0);
		if (result == SOCKET_ERROR) {
			// log error
			return false;
		}

		length = result;
		return result;
	}
	bool UDPSocket::recv(char* buf, int buflen, bool peek) {
		int temp;
		return recv(buf, buflen, temp, peek);
	}
	Host UDPSocket::recv_from(char* buf, int buflen, int& length, bool peek) {
		sockaddr_in addr;
		socklen_t addr_len;
		addr_len = sizeof(addr);
		memset(&addr, 0, sizeof(addr));

		int result = ::recvfrom(_socket, buf, buflen, peek ? MSG_PEEK : 0, (sockaddr*)&addr, &addr_len);
		if (result == SOCKET_ERROR) {
			// log error
			return Host(IPv4((IPv4::binary_type)0));
		}

		length = result;
		return Host(IPv4(addr.sin_addr));
	}
	Host UDPSocket::recv_from(char* buf, int buflen, bool peek) {
		int temp;
		return recv_from(buf, buflen, temp, peek);
	}

	bool UDPSocket::broadcast_enabled() const {
		int optval;
		socklen_t optlen = sizeof(optval);

		int result = getsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, &optlen);
		if (result != 0) {
			// log error
			return 0;
		}

		return optval;
	}
	void UDPSocket::broadcast_enabled(bool new_val) {
		int optval = new_val;

		int result = setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval));
		if (result != 0) {
			// log error;
		}
	}
}