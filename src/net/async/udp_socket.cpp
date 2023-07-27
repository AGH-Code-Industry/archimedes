#include <net/async/udp_socket.hpp>

namespace arch::net::async {
	UDPSocket::UDPSocket() :
		_base() {
#ifdef WIN32
		u_long arg = 1;
		ioctlsocket(_socket, FIONBIO, &arg);
#elif defined unix
		int arg = 1;
		ioctl(_socket, FIONBIO, &arg);
#endif
	}
	UDPSocket::UDPSocket(port_type port) :
		_base(port) {
#ifdef WIN32
		u_long arg = 1;
		ioctlsocket(_socket, FIONBIO, &arg);
#elif defined unix
		int arg = 1;
		ioctl(_socket, FIONBIO, &arg);
#endif
	}
	UDPSocket::UDPSocket(IPv4 address, port_type port) :
		_base(address, port) {
#ifdef WIN32
		u_long arg = 1;
		ioctlsocket(_socket, FIONBIO, &arg);
#elif defined unix
		int arg = 1;
		ioctl(_socket, FIONBIO, &arg);
#endif
	}
	UDPSocket::~UDPSocket() {
		_base::~UDPSocket();
	}
	
	std::future<bool> UDPSocket::send_to(const Host& host, port_type port, const char* data, int len) {
		return std::async(std::launch::async, [this](const net::Host& _host, port_type _port, const char* _data, int _len)->bool {
			return _base::send_to(_host, _port, _data, _len);
		}, host.sync(), port, data, len);
	}
	std::future<bool> UDPSocket::send_to(const Host& host, const char* data, int len) {
		return std::async(std::launch::async, [this](const net::Host& _host, const char* _data, int _len)->bool {
			return _base::send_to(_host, _data, _len);
		}, host.sync(), data, len);
	}
	std::future<bool> UDPSocket::send_to(const Host& host, port_type port, std::string_view data) {
		return std::async(std::launch::async, [this](const net::Host& _host, port_type _port, std::string_view _data)->bool {
			return _base::send_to(_host, _port, _data);
		}, host.sync(), port, data);
	}
	std::future<bool> UDPSocket::send_to(const Host& host, std::string_view data) {
		return std::async(std::launch::async, [this](const net::Host& _host, std::string_view _data)->bool {
			return _base::send_to(_host, _data);
		}, host.sync(), data);
	}

	std::future<bool> UDPSocket::recv(char* buf, int buflen, bool peek) {
		return std::async(std::launch::async, [this](char* _buf, int _buflen, bool _peek)->bool {
			
		}, buf, buflen, peek);
	}
	std::future<Host> UDPSocket::recv_from(char* buf, int buflen, bool peek) {
		return std::async(std::launch::async, [this](char* _buf, int _buflen, bool _peek)->Host {
			return _base::recv_from(_buf, _buflen, _peek).async();
		}, buf, buflen, peek);
	}
}