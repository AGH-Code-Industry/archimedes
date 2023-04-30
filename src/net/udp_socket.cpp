#include <net/udp_socket.hpp>

namespace arch::net {
	UDPSocket::UDPSocket() :
		Socket(Socket::UDP) {

	}
	UDPSocket::UDPSocket(port_type port) :
		Socket(Socket::UDP, port) {

	}
	UDPSocket::UDPSocket(IPv4 address, port_type port) :
		Socket(Socket::UDP, address, port) {

	}
}