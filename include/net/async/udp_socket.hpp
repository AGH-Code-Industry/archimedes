#pragma once

#include <net/async/host.hpp>
#include <net/udp_socket.hpp>

namespace arch::net::async {
	class UDPSocket : protected net::UDPSocket {
	public:
		/// @brief Default constructor.
		///
		UDPSocket();
		/// @brief Any-binding constructor
		/// @details Binds to all interfaces using INADDR_ANY.
		UDPSocket(port_type port);
		/// @brief Binding constructor.
		///
		UDPSocket(IPv4 address, port_type port);
		/// @brief Deleted copy-constructor.
		///
		UDPSocket(const UDPSocket&) = delete;
		/// @brief Deleted copy-constructor.
		///
		UDPSocket(UDPSocket&) = delete;
		/// @brief Default move-constructor.
		///
		UDPSocket(UDPSocket&&) = default;
		/// @brief Destructor.
		///
		~UDPSocket();

		/// @brief Deleted assignment operator.
		///
		UDPSocket& operator=(const UDPSocket&) = delete;
		/// @brief Deleted assignment operator.
		///
		UDPSocket& operator=(UDPSocket&) = delete;
		/// @brief Deleted assignment operator.
		///
		UDPSocket& operator=(UDPSocket&&) = delete;

		using net::UDPSocket::address;
		using net::UDPSocket::bind;
		using net::UDPSocket::bound;
		using net::UDPSocket::broadcast_enabled;
		using net::UDPSocket::close;
		using net::UDPSocket::data_avalible;
		using net::UDPSocket::exclusive;
		using net::UDPSocket::port;
		using net::UDPSocket::protocol;
		using net::UDPSocket::recv_buf;
		using net::UDPSocket::reuse;
		using net::UDPSocket::sendable;
		using net::UDPSocket::send_buf;
		using net::UDPSocket::usable;
		using net::UDPSocket::usable_data;

		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param port - port to send data to.
		/// @param data - data to be sent.
		/// @param length - length of data to be sent.
		/// @return std::future with info if succeeded.
		std::future<bool> send_to(const Host& host, port_type port, const char* data, int len);
		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param data - data to be sent.
		/// @param length - length of data to be sent.
		/// @return std::future with info if succeeded.
		std::future<bool> send_to(const Host& host, const char* data, int len);
		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param port - port to send data to.
		/// @param data - data to be sent.
		/// @return std::future with info if succeeded.
		std::future<bool> send_to(const Host& host, port_type port, std::string_view data);
		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param data - data to be sent.
		/// @return std::future with info if succeeded.
		std::future<bool> send_to(const Host& host, std::string_view data);

		/// @brief Asynchronously receives data.
		/// @param buf - buffer to save data to.
		/// @param buflen - length of buffer.
		/// @param timeout - timeout.
		/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
		/// @return std::future with result.
		std::future<bool> recv(char* buf, int buflen, timeout_t timeout, bool peek = false);
		/// @brief Asynchronously receives data and sender.
		/// @param buf - buffer to save data to.
		/// @param buflen - length of buffer.
		/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
		/// @return std::future with sender host.
		std::future<Host> recv(char* buf, int buflen, bool peek = false);

	private:
		using _base = net::UDPSocket;
	};
}

namespace arch::net {
	//using AsyncUDPSocket = async::UDPSocket;
}