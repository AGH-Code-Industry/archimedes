#pragma once

#include <net/includes.hpp>
#include <net/ipv4.hpp>
#include <functional>

namespace arch::net {
	/// @brief Represents general socket.
	///
	class Socket {
	public:
		/// @brief Enum of avalible socket types.
		///
		enum sock_protocol : uint16_t {
			None,
			UDP,
			TCP
		};

		/// @brief Build-in socket handle type.
		///
		using sock_type = std::function<decltype(socket)>::result_type;
		/// @brief Port number type.
		///
		using port_type = uint16_t;

		/// @brief IPv4 constant to signalize binding to all interfaces.
		///
		static const IPv4 any_address;
		/// @brief port_type constatnt to signalize random binding port.
		/// @details Port number can be accessed by port() method.
		static const port_type random_port;

		/// @brief Constructor.
		/// @details Socket needs to be bind using bind() method.
		Socket(sock_protocol p);
		/// @brief Binding constructor.
		///
		Socket(sock_protocol p, IPv4 address, port_type port);
		/// @brief Any-binding constructor
		/// @details Binds to all interfaces using INADDR_ANY.
		Socket(sock_protocol p, port_type port);
		/// @brief Deleted copy-constructor.
		///
		Socket(const Socket&) = delete;
		/// @brief Deleted copy-constructor.
		///
		Socket(Socket&) = delete;
		/// @brief Default move-constructor.
		///
		Socket(Socket&&) = default;
		/// @brief Destructor. Automaticly closes socket.
		///
		~Socket();

		/// @brief Deleted assignment operator.
		///
		Socket& operator=(const Socket&) = delete;
		/// @brief Deleted assignment operator.
		///
		Socket& operator=(Socket&) = delete;
		/// @brief Deleted assignment operator.
		///
		Socket& operator=(Socket&&) = delete;

		/// @brief Closes socket.
		///
		void close();
		/// @brief Binds socket to given IPv4 and port.
		/// @return true on success, false otherwise.
		bool bind(IPv4 address, port_type port);
		/// @brief Binds socket to all interfaces and given port.
		/// @return true on success, false otherwise.
		bool bind(port_type port);
		/// @brief Binds to all interfaces and random port.
		/// @return Bound port number on success, 0 on failure.
		port_type bind();

		/// @brief Returns address of socket or any_address if bound to all interfaces.
		///
		IPv4 address() const;
		/// @brief Returns port number of socket or 0 if not bound.
		///
		port_type port() const;
		/// @brief Returns protocol of this socket.
		///
		sock_protocol protocol() const;
		/// @brief Checks if socket is bound.
		///
		bool bound() const;

	protected:
		sock_type _socket = NULL;
		IPv4 _address = 0;
		port_type _port = 0;
		sock_protocol _proto = sock_protocol::None;
	};
}