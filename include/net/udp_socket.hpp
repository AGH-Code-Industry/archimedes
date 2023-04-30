#pragma once

#include <net/socket.hpp>

namespace arch::net {
	/// @brief Represents UDP socket.
	///
	class UDPSocket : Socket {
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
	};
}