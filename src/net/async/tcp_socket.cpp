#include <net/async/tcp_socket.hpp>
#include <net/exception.hpp>

namespace arch::net::async {
TCPSocket::TCPSocket() :
	_base() {}
TCPSocket::TCPSocket(port_type port) :
	_base(port) {}
TCPSocket::TCPSocket(IPv4 address, port_type port) :
	_base(address, port) {}
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
		std::scoped_lock lock(_send_mutex, _recv_mutex);
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
			throw NetException("only listening sockets can accept()");
		}

		sockaddr_in sock;
		socklen_t socklen = sizeof(sock);
		memset(&sock, 0, sizeof(sock));
		auto result = ::accept(_socket, (sockaddr*)&sock, &socklen);
		if (result == INVALID_SOCKET) {
			throw NetException(gai_strerror(net_errno()));
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
		std::scoped_lock lock(_send_mutex, _recv_mutex);
		TCPSocket& _ns = ns.get();

		auto accept_future = accept(_ns);

		_ns._status = 0;
		auto buffer = std::unique_ptr<char[]>(new char[dl]);
		memset(buffer.get(), 0, dl);
		// receive connection data
		int result = ::recv(_ns._socket, buffer.get(), dl, 0);
		if (result == SOCKET_ERROR) {
			throw NetException(std::string("while receiving connection data: ") + gai_strerror(net_errno()));
		}
		else if (result == 0) {
			throw NetException(std::string("peer disconnected before sending connection data"));
		}
		auto resp_buffer = std::unique_ptr<char[]>(new char[rl]);
		memset(resp_buffer.get(), 0, rl);
		auto return_result = c(buffer.get(), dl, ad, resp_buffer.get(), rl);
		result = ::send(_ns._socket, resp_buffer.get(), rl, 0);
		if (result == SOCKET_ERROR) {
			throw NetException(std::string("while sending connection data: ") + gai_strerror(net_errno()));
		}
		if (return_result) {
			_ns._status = 1;
			return true;
		}

		return false;
	}, std::ref(new_sock), condition, data_len, response_len, additional_data);
}

std::future<bool> TCPSocket::send(const char* data, int len) {
	std::lock_guard lock(_send_mutex);
	return std::async(std::launch::async, [this](const char* d, int l)->bool {
		std::lock_guard lock(_send_mutex);
		return _base::send(d, l);
	}, data, len);
}
std::future<bool> TCPSocket::send(const std::string& data) {
	return std::async(std::launch::async, [this](const std::string& d)->bool {
		std::lock_guard lock(_send_mutex);
		return _base::send(d);
	}, data);
}

std::future<bool> TCPSocket::recv(char* buf, int buflen, int& length, timeout_t timeout, bool peek) {
	return std::async(std::launch::async, [this](char* b, int bl, std::reference_wrapper<int> l, timeout_t t, bool p)->bool {
		std::chrono::milliseconds time_left;
		if (t < 0) { // timeout == inf
			_recv_mutex.lock();
			time_left = std::chrono::milliseconds(t);
		}
		else { // timeout != inf
			auto timer = std::chrono::high_resolution_clock::now();
			if (_recv_mutex.try_lock_for(std::chrono::milliseconds(t))) {
				time_left = std::chrono::milliseconds(t) - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - timer);
			}
			else {
				return false;
			}
		}

		pollfd poll_data{};
		poll_data.fd = _socket;
		poll_data.events = POLLRDNORM;
		poll_data.revents = 0;
		int result = poll(&poll_data, 1, time_left.count());

		if (result == SOCKET_ERROR) {
			throw NetException(gai_strerror(net_errno()));
		}
		else if (result == 0) { // timeout expired
			_recv_mutex.unlock();
			return false;
		}
		else if (poll_data.revents & POLLRDNORM) { // read
			try {
				auto res = _base::recv(b, bl, l.get(), p);
				_recv_mutex.unlock();
				return res;
			}
			catch (NetException e) {
				_recv_mutex.unlock();
				throw e;
			}
		}
		else {
			_recv_mutex.unlock();
			return false;
		}
	}, buf, buflen, std::ref(length), timeout, peek);
}
std::future<bool> TCPSocket::recv(char* buf, int buflen, timeout_t timeout, bool peek) {
	int temp;
	return recv(buf, buflen, temp, timeout, peek);
}
std::future<bool> TCPSocket::recv(char* buf, int buflen, int& length, bool peek) {
	return recv(buf, buflen, length, -1, peek);
}
std::future<bool> TCPSocket::recv(char* buf, int buflen, bool peek) {
	int temp;
	return recv(buf, buflen, temp, peek);
}
}