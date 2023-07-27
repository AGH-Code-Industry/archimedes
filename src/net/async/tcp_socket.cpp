#include <net/async/tcp_socket.hpp>

namespace arch::net::async {
	TCPSocket::TCPSocket() :
		_base() {
#ifdef WIN32
		u_long arg = 1;
		ioctlsocket(_socket, FIONBIO, &arg);
#elif defined unix
		int arg = 1;
		ioctl(_socket, FIONBIO, &arg);
#endif
	}
	TCPSocket::TCPSocket(port_type port) :
		_base(port) {
#ifdef WIN32
		u_long arg = 1;
		ioctlsocket(_socket, FIONBIO, &arg);
#elif defined unix
		int arg = 1;
		ioctl(_socket, FIONBIO, &arg);
#endif
	}
	TCPSocket::TCPSocket(IPv4 address, port_type port) :
		_base(address, port) {
#ifdef WIN32
		u_long arg = 1;
		ioctlsocket(_socket, FIONBIO, &arg);
#elif defined unix
		int arg = 1;
		ioctl(_socket, FIONBIO, &arg);
#endif
	}
	TCPSocket::~TCPSocket() {
		Socket::~Socket();
	}

	std::future<bool> TCPSocket::connected_force() {
		return std::async(std::launch::async, [this]()->bool {
			return _base::connected_force();
		});
	}

	std::future<bool> TCPSocket::connect(const Host& host, port_type port) {
		return std::async(std::launch::async, [this](const Host& h, port_type p)->bool {
			return _base::connect(h.sync(), p);
		}, host, port);
	}
	std::future<bool> TCPSocket::cond_connect(const Host& host, port_type port, void* data, int data_len, int response_len, accept_response_handler handler, void* handler_data) {
		return std::async(std::launch::async, [this](const Host& h, port_type p, void* d, int dl, int rl, accept_response_handler h2, void* hd = nullptr)->bool {
			return _base::cond_connect(h.sync(), p, d, dl, rl, h2, hd);
		}, host, port, data, data_len, response_len, handler, handler_data);
	}
	std::future<bool> TCPSocket::listen() {
		return std::async(std::launch::async, [this]()->bool {
			return _base::listen();
		});
	}
	std::future<bool> TCPSocket::listen(int maxconn) {
		return std::async(std::launch::async, [this](int m)->bool {
			return _base::listen(m);
		}, maxconn);
	}
	std::future<bool> TCPSocket::accept(TCPSocket& new_sock) {
		return std::async(std::launch::async, [this](std::reference_wrapper<TCPSocket> ns)->bool {
			TCPSocket& _ns = ns.get();

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

			_ns._address = _address;
			_ns._peer_addr = IPv4(sock.sin_addr);
			_ns._port = ntohs(sock.sin_port);
			_ns._proto = Socket::protocol_t::TCP;
			_ns._socket = result;
			_ns._status = 1;

			return true;
		}, std::ref(new_sock));
	}
	std::future<bool> TCPSocket::cond_accept(TCPSocket& new_sock, accept_condition condition, int data_len, int response_len, void* additional_data) {
		return std::async(std::launch::async, [this](std::reference_wrapper<TCPSocket> ns, accept_condition c, int dl, int rl, void* ad)->bool {
			TCPSocket& _ns = ns.get();

			auto accept_future = accept(_ns);

			if (not accept_future.get()) {
				// log error
				return false;
			}

			_ns._status = 0;
			auto buffer = std::unique_ptr<char[]>(new char[dl]);
			memset(buffer.get(), 0, dl);
			// receive connection data
			int result = ::recv(_ns._socket, buffer.get(), dl, 0);
			if (result == SOCKET_ERROR) {
				// log error
				return false;
			}
			auto resp_buffer = std::unique_ptr<char[]>(new char[rl]);
			memset(resp_buffer.get(), 0, rl);
			auto return_result = c(buffer.get(), dl, ad, resp_buffer.get(), rl);
			result = ::send(_ns._socket, resp_buffer.get(), rl, 0);
			if (result == SOCKET_ERROR) {
				// log error
				return false;
			}
			if (return_result) {
				_ns._status = 1;
				return true;
			}

			return false;
		}, std::ref(new_sock), condition, data_len, response_len, additional_data);
	}

	std::future<bool> TCPSocket::send(const char* data, int len) {
		return std::async(std::launch::async, [this](const char* d, int l)->bool {
			return _base::send(d, l);
		}, data, len);
	}
	std::future<bool> TCPSocket::send(std::string_view data) {
		return std::async(std::launch::async, [this](std::string_view d)->bool {
			return _base::send(d);
		}, data);
	}

	std::future<bool> TCPSocket::recv(char* buf, int buflen, int& length, bool peek) {
		return std::async(std::launch::async, [this](char* b, int bl, std::reference_wrapper<int> l, bool p)->bool {
			return _base::recv(b, bl, l.get(), p);
		}, buf, buflen, std::ref(length), peek);
	}
	std::future<bool> TCPSocket::recv(char* buf, int buflen, bool peek) {
		return std::async(std::launch::async, [this](char* b, int bl, bool p)->bool {
			return _base::recv(b, bl, p);
		}, buf, buflen, peek);
	}
}