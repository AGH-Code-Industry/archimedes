#pragma once

#include <net/socket.hpp>
#include <net/host.hpp>
#include <chrono>

namespace arch::net {
class Serializable;

/// @brief Represents TCP sockets.
///
class TCPSocket : public Socket {
public:
	/// @brief Predicate to cond_accept().
	/// @param void* - pointer to acceptance data.
	/// @param int - length of acceptance data in bytes.
	/// @param void* - pointer to additional data used by predicate.
	/// @param void* - pointer to response buffer.
	/// @param int - response buffer length.
	/// @retval true if connection accepted, false otherwise.
	using accept_condition = bool(*)(void*, int, void*, void*, int);
	/// @brief Predicate to cond_connect().
	/// @param void* - response data.
	/// @param int - response length.
	/// @param void* - additional data.
	using accept_response_handler = bool(*)(void*, int, void*);

	/// @brief data used by linger() methods.
	///
	struct linger_data {
		bool linger;
		decltype(linger::l_linger) seconds;
	};

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

	/// @brief Returns current linger data of socket.
	///
	linger_data linger() const;
	/// @brief Sets linger data of socket.
	/// @param data - linger data
	void linger(linger_data data);

	/// @brief Requests unconditional connection.
	/// @param host - host, to which ip() connection is requested.
	/// @param port - port on which connection is requested.
	/// @return true on success, false otherwise.
	bool connect(const Host& host, port_type port);
	/// @brief Requests conditional connection.
	/// @param host - host, to which ip() connection is requested.
	/// @param port - port on which connection is requested.
	/// @param data - acceptance data to be sent.
	/// @param data_len - length of acceptance data.
	/// @param response_len - length of response data.
	/// @param handler - pointer to response handler.
	/// @param handler_data - additional data for handler.
	/// @return true on success, false otherwise.
	bool cond_connect(const Host& host, port_type port, void* data, int data_len, int response_len, accept_response_handler handler, void* handler_data = nullptr);
	/// @brief Checks if socket was connected as of last operation.
	///
	bool connected() const;
	/// @brief Checks if socket is connected.
	/// 
	bool connected_force();

	/// @brief Puts socket into listening mode.
	/// @return true on success, false otherwise.
	bool listen();
	/// @brief Puts socket into listening mode.
	/// @param maxconn - max size of accpet queue.
	/// @return true on success, false otherwise.
	bool listen(int maxconn);
	/// @brief Checks if socket is listening.
	/// @return 
	bool listening() const;

	/// @brief Unconditionally accepts incoming connection.
	/// @param new_sock - socket object that will hold connection socket.
	/// @return true on success, false otherwise. 
	bool accept(TCPSocket& new_sock);
	/// @brief Conditionally accepts incoming connection.
	/// @param new_sock - socket object that will hold connection socket.
	/// @param condition - callback to predicate.
	/// @param data_len - length of acceptance data.
	/// @param additional_data - additional data used by predicate.
	/// @return true on success, false otherwise. 
	bool cond_accept(TCPSocket& new_sock, accept_condition condition, int data_len, int response_len, void* additional_data = nullptr);

	/// @brief Sends data to peer.
	/// @param data - data to be sent.
	/// @param len - length of data to be sent.
	/// @return true on success, false otherwise.
	bool send(const char* data, int len);
	/// @brief Sends data to peer.
	/// @param data - data to be sent.
	/// @return true on success, false otherwise.
	bool send(std::string_view data);

	/// @brief Receives data.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param length - variable that will contain length of received data in bytes, -1 on error.
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	bool recv(char* buf, int buflen, int& length, bool peek = false);
	/// @brief Receives data.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise. 
	bool recv(char* buf, int buflen, bool peek = false);

protected:

	IPv4 _peer_addr;
	uint8_t _status = 0;
};
}