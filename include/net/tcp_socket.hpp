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
		using accept_condition = bool(*)(void*, int, void*);
		/// @brief No connection condition placeholder, always returns true.
		///
		constexpr static bool no_condition(void* = nullptr, int = 0, void* = nullptr);

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
		~TCPSocket() = default;

		/// @brief Deleted assignment operator.
		///
		TCPSocket& operator=(const TCPSocket&) = delete;
		/// @brief Deleted assignment operator.
		///
		TCPSocket& operator=(TCPSocket&) = delete;
		/// @brief Deleted assignment operator.
		///
		TCPSocket& operator=(TCPSocket&&) = delete;

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
		/// @return true on success, false otherwise.
		bool cond_connect(const Host& host, port_type port, void* data, int data_len) = delete;
		/// @brief Checks if socket is connected.
		///
		bool connected() const;

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
		/// @param additional_data - additional data used by predicate.
		/// @return true on success, false otherwise. 
		bool cond_accept(TCPSocket& new_sock, accept_condition condition, void* additional_data = nullptr) = delete;

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
		//TCPSocket(sock_type s, port_type p, IPv4 addr, IPv4 paddr);

		IPv4 _peer_addr;
		uint8_t _status = 0;
	};
}