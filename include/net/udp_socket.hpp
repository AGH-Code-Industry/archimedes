#pragma once

#include <net/socket.hpp>
#include <net/host.hpp>

namespace arch::net {
	class Serializable;

	/// @brief Represents synchronous UDP socket.
	///
	class UDPSocket : public Socket {
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
		~UDPSocket() = default;

		/// @brief Deleted assignment operator.
		///
		UDPSocket& operator=(const UDPSocket&) = delete;
		/// @brief Deleted assignment operator.
		///
		UDPSocket& operator=(UDPSocket&) = delete;
		/// @brief Deleted assignment operator.
		///
		UDPSocket& operator=(UDPSocket&&) = delete;

		/// @brief Check if socket is able to send broadcast.
		///
		bool broadcast_enabled() const;
		/// @brief Set if socket is able to send broadcast.
		///
		void broadcast_enabled(bool val);

		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param port - port to send data to.
		/// @param data - data to be sent.
		/// @param length - length of data to be sent.
		/// @return true on success, false on any error.
		bool send_to(const Host& host, port_type port, const char* data, int len);
		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param data - data to be sent.
		/// @param length - length of data to be sent.
		/// @return true on success, false on any error.
		bool send_to(const Host& host, const char* data, int len);
		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param port - port to send data to.
		/// @param data - data to be sent.
		/// @return true on success, false on any error.
		bool send_to(const Host& host, port_type port, std::string_view data);
		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param data - data to be sent.
		/// @return true on success, false on any error.
		bool send_to(const Host& host, std::string_view data);
		/// @brief Sends given data to given host.
		/// @details If data length exceeds sendbuff_size(), error occurs.
		/// @param host - host to send data to, ip() will be used as address.
		/// @param serializable - object to be serialized and sent.
		/// @return true on success, false on any error.
		//bool send_to(const Host& host, Serializable* serializable) = delete; // to update after serialization

		/// @brief Receives data.
		/// @param buf - buffer to save data to.
		/// @param buflen - length of buffer.
		/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
		/// @return true if received data, false otherwise.
		bool recv(char* buf, int buflen, bool peek = false);
		/// @brief Receives data and sender.
		/// @param buf - buffer to save data to.
		/// @param buflen - length of buffer.
		/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
		/// @return Unupdated sender host on success, Host("0.0.0.0") otherwise.
		Host recv_from(char* buf, int buflen, bool peek = false);
	};
}