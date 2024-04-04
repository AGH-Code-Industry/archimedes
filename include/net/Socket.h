#pragma once

#include <type_traits>

#include <net/IPv4.h>
#include <net/Utilities.h>

namespace arch::net {
class UDPSocket;
class TCPSocket;

/// @brief Represents general IPv4 socket. Not meant to be used
///
class Socket {
public:
	/// @brief Enum of avalible socket types.
	///
	enum class Protocol : uint16_t {
		none,
		udp,
		tcp
	};

	/// @brief UDP socket.
	///
	using UDP = UDPSocket;
	/// @brief TCP socket.
	///
	using TCP = TCPSocket;

	/// @brief Structure used in usable() method.
	///
	struct UsableData {
		bool dataAvalible;
		bool sendable;
	};

	/// @brief Build-in socket handle type.
	///
	using SockType = std::invoke_result_t<decltype(socket), int, int, int>;
	/// @brief Port number type.
	///
	using Port = uint16_t;

	/// @brief IPv4 constant to signalize binding to all interfaces.
	///
	static const IPv4 anyAddress;
	/// @brief Port constant to signalize random binding port.
	/// @details Port number can be accessed by port() method.
	static const Port randomPort;

	/// @brief Deleted assignment operator.
	///
	Socket& operator=(const Socket&) = delete;
	/// @brief Deleted assignment operator.
	///
	Socket& operator=(Socket&) = delete;
	/// @brief Defaulted move assignment operator.
	///
	Socket& operator=(Socket&&) = default;

	/// @brief Returns address of socket or any_address if bound to all interfaces.
	///
	IPv4 address() const;
	/// @brief Returns port number of socket or 0 if not bound.
	///
	Port port() const;
	/// @brief Returns protocol of this socket.
	///
	Protocol protocol() const;
	/// @brief Checks if socket is bound.
	///
	bool bound() const;
	/// @brief Checks if there is data avalible to read.
	///
	bool dataAvalible() const;
	/// @brief Checks if socket is avalible for sendung data.
	///
	bool sendable() const;
	/// @brief Behaves like data_avalible() and sendable() called at once.
	/// @return UsableData structure containting information.
	UsableData usable() const;

	/// @brief Closes socket.
	///
	void close();
	/// @brief Binds socket to given IPv4 and port.
	/// @return true on success, false otherwise.
	bool bind(IPv4 address, Port port);
	/// @brief Binds socket to all interfaces and given port.
	/// @return true on success, false otherwise.
	bool bind(Port port);
	/// @brief Binds to all interfaces and random port.
	/// @return Bound port number on success, 0 on failure.
	Port bind();

	/// @brief Checks receiving buffer size.
	/// @retval Size of buffer on success, 0 otherwise.
	int recvBuf() const;
	/// @brief Sets receiving buffer size.
	/// @param newVal - new size of receive buffer.
	void recvBuf(int newVal);
	/// @brief Checks sending buffer size.
	/// @retval Size of buffer on success, 0 otherwise.
	int sendBuf() const;
	/// @brief Sets sending buffer size.
	/// @param newVal - new size of sending buffer.
	void sendBuf(int newVal);
#if ARCHIMEDES_WINDOWS // exclusivity avalible only on Windows :(
	/// @brief Checks if address and port of socket should be exclusive.
	/// @details Avalible only on Windows.
	bool exclusive() const;
	/// @brief Sets if address and port of socket should be exclusive. Must be set before binding.
	/// @details Avalible only on Windows.
	void exclusive(bool newVal);
#endif
	/// @brief Checks if socket is permitted to use address and port of another non-exclusive socket.
	///
	bool reuse() const;
	/// @brief Sets if socket is permitted to use address and port of another non-exclusive socket.
	///
	void reuse(bool newVal);

protected:
	Socket(Protocol protocol);
	Socket(Protocol protocol, IPv4 address, Port port);
	Socket(Protocol protocol, Port port);
	Socket(const Socket&) = delete;
	Socket(Socket&) = delete;
	Socket(Socket&&) = default;
	~Socket();

	SockType _socket = INVALID_SOCKET;
	IPv4 _address{ (uint32_t)0 };
	Port _port = 0;
	Protocol _proto = Protocol::none;
};
} // namespace arch::net
