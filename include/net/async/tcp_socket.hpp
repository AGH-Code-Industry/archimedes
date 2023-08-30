#pragma once

#include <net/async/host.hpp>
#include <net/tcp_socket.hpp>

namespace arch::net::async {
/// @brief Represents asynchronous TCP sockets.
///
class TCPSocket : private net::TCPSocket {
public:
	using net::TCPSocket::accept_condition;
	using net::TCPSocket::accept_response_handler;
	using net::TCPSocket::linger_data;

	/// @brief Default constructor.
	///
	TCPSocket();
	/// @brief Any-binding constructor
	/// @details Binds to all interfaces using INADDR_ANY.
	TCPSocket(port_type port);
	/// @brief Binding constructor.
	///
	TCPSocket(IPv4 address, port_type port);
	/// @brief Deleted copy-constructor.
	///
	TCPSocket(const TCPSocket&) = delete;
	/// @brief Deleted copy-constructor.
	///
	TCPSocket(TCPSocket&) = delete;
	/// @brief Default move-constructor.
	///
	TCPSocket(TCPSocket&&) = default;
	/// @brief Destructor.
	///
	~TCPSocket();

	/// @brief Deleted assignment operator.
	///
	TCPSocket& operator=(const TCPSocket&) = delete;
	/// @brief Deleted assignment operator.
	///
	TCPSocket& operator=(TCPSocket&) = delete;
	/// @brief Deleted assignment operator.
	///
	TCPSocket& operator=(TCPSocket&&) = delete;

	using net::TCPSocket::linger;
	using net::TCPSocket::connected;
	/// @brief Checks if socket is connected.
	/// 
	std::future<bool> connected_force();
	using net::TCPSocket::listening;
	using net::TCPSocket::address;
	using net::TCPSocket::bind;
	using net::TCPSocket::bound;
	using net::TCPSocket::close;
	using net::TCPSocket::data_avalible;
	using net::TCPSocket::exclusive;
	using net::TCPSocket::port;
	using net::TCPSocket::protocol;
	using net::TCPSocket::recv_buf;
	using net::TCPSocket::reuse;
	using net::TCPSocket::sendable;
	using net::TCPSocket::send_buf;
	using net::TCPSocket::usable;
	using net::TCPSocket::usable_data;

	/// @brief Requests unconditional connection.
	/// @param host - host, to which ip() connection is requested.
	/// @param port - port on which connection is requested.
	/// @return true on success, false otherwise.
	std::future<bool> connect(const Host& host, port_type port);
	/// @brief Requests conditional connection.
	/// @param host - host, to which ip() connection is requested.
	/// @param port - port on which connection is requested.
	/// @param data - acceptance data to be sent.
	/// @param data_len - length of acceptance data.
	/// @param response_len - length of response data.
	/// @param handler - pointer to response handler.
	/// @param handler_data - additional data for handler.
	/// @return true on success, false otherwise.
	std::future<bool> cond_connect(const Host& host, port_type port, void* data, int data_len, int response_len, accept_response_handler handler, void* handler_data = nullptr);

	/// @brief Puts socket into listening mode.
	/// @return true on success, false otherwise.
	std::future<bool> listen();
	/// @brief Puts socket into listening mode.
	/// @param maxconn - max size of accpet queue.
	/// @return true on success, false otherwise.
	std::future<bool> listen(int maxconn);

	/// @brief Unconditionally accepts incoming connection.
	/// @param new_sock - socket object that will hold connection socket.
	/// @return true on success, false otherwise. 
	std::future<bool> accept(TCPSocket& new_sock);
	/// @brief Conditionally accepts incoming connection.
	/// @param new_sock - socket object that will hold connection socket.
	/// @param condition - callback to predicate.
	/// @param data_len - length of acceptance data.
	/// @param additional_data - additional data used by predicate.
	/// @return true on success, false otherwise. 
	std::future<bool> cond_accept(TCPSocket& new_sock, accept_condition condition, int data_len, int response_len, void* additional_data = nullptr);

	/// @brief Sends data to peer.
	/// @param data - data to be sent.
	/// @param len - length of data to be sent.
	/// @return true on success, false otherwise.
	std::future<bool> send(const char* data, int len);
	/// @brief Sends data to peer.
	/// @param data - data to be sent.
	/// @return true on success, false otherwise.
	std::future<bool> send(const std::string& data);

	/// @brief Asynchronously receives data.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param length - variable that will contain length of received data in bytes, -1 on error.
	/// @param timeout - timeout, if negative wait indefinetly
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	std::future<bool> recv(char* buf, int buflen, int& length, timeout_t timeout, bool peek = true);
	/// @brief Asynchronously receives data.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param timeout - timeout, if negative wait indefinetly
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	std::future<bool> recv(char* buf, int buflen, timeout_t timeout, bool peek = true);
	/// @brief Asynchronously receives data, waits indefinetely.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param length - variable that will contain length of received data in bytes, -1 on error.
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	std::future<bool> recv(char* buf, int buflen, int& length, bool peek = false);
	/// @brief Asynchronously receives data, waits indefinetely.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise. 
	std::future<bool> recv(char* buf, int buflen, bool peek = false);

private:
	using _base = net::TCPSocket;
	std::mutex _send_mutex;
	std::timed_mutex _recv_mutex;
};
}