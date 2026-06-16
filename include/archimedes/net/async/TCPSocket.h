#pragma once

#include <archimedes/net/TCPSocket.h>
#include <archimedes/net/async/Host.h>

namespace arch::net::async {
/// @brief Represents asynchronous TCP sockets.
///
class TCPSocket: private net::TCPSocket {
public:
	using net::TCPSocket::AcceptCondition;
	using net::TCPSocket::AcceptResponseHandler;
	using net::TCPSocket::LingerData;

	/// @brief Default constructor.
	///
	TCPSocket();
	/// @brief Any-binding constructor
	/// @details Binds to all interfaces using INADDR_ANY.
	TCPSocket(Port port);
	/// @brief Binding constructor.
	///
	TCPSocket(IPv4 address, Port port);
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

	using net::TCPSocket::connected;
	using net::TCPSocket::linger;
	/// @brief Checks if socket is connected.
	///
	std::future<bool> connectedForce();
	using net::TCPSocket::address;
	using net::TCPSocket::bind;
	using net::TCPSocket::bound;
	using net::TCPSocket::close;
	using net::TCPSocket::dataAvalible;
	using net::TCPSocket::listening;
#if ARCHIMEDES_WINDOWS
	using net::TCPSocket::exclusive;
#endif
	using net::TCPSocket::port;
	using net::TCPSocket::protocol;
	using net::TCPSocket::recvBuf;
	using net::TCPSocket::reuse;
	using net::TCPSocket::sendable;
	using net::TCPSocket::sendBuf;
	using net::TCPSocket::usable;
	using net::TCPSocket::UsableData;

	/// @brief Requests unconditional connection.
	/// @param host - host, to which ip() connection is requested.
	/// @param port - port on which connection is requested.
	/// @return true on success, false otherwise.
	std::future<bool> connect(const Host& host, Port port);
	/// @brief Requests conditional connection.
	/// @param host - host, to which ip() connection is requested.
	/// @param port - port on which connection is requested.
	/// @param data - acceptance data to be sent.
	/// @param dataLen - length of acceptance data.
	/// @param responseLen - length of response data.
	/// @param handler - pointer to response handler.
	/// @param handlerData - additional data for handler.
	/// @return true on success, false otherwise.
	std::future<bool> condConnect(
		const Host& host,
		Port port,
		void* data,
		int dataLen,
		int responseLen,
		AcceptResponseHandler handler,
		void* handlerData = nullptr
	);

	/// @brief Puts socket into listening mode.
	/// @return true on success, false otherwise.
	std::future<bool> listen();
	/// @brief Puts socket into listening mode.
	/// @param maxconn - max size of accpet queue.
	/// @return true on success, false otherwise.
	std::future<bool> listen(int maxconn);

	/// @brief Unconditionally accepts incoming connection.
	/// @param newSock - socket object that will hold connection socket.
	/// @return true on success, false otherwise.
	std::future<bool> accept(TCPSocket& newSock);
	/// @brief Conditionally accepts incoming connection.
	/// @param newSock - socket object that will hold connection socket.
	/// @param condition - callback to predicate.
	/// @param dataLen - length of acceptance data.
	/// @param responseLen - length of response data.
	/// @param additionalData - additional data used by predicate.
	/// @return true on success, false otherwise.
	std::future<bool> condAccept(
		TCPSocket& newSock,
		AcceptCondition condition,
		int dataLen,
		int responseLen,
		void* additionalData = nullptr
	);

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
	std::future<bool> recv(char* buf, int buflen, int& length, TimeoutMs timeout, bool peek = true);
	/// @brief Asynchronously receives data.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param timeout - timeout, if negative wait indefinetly
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	std::future<bool> recv(char* buf, int buflen, TimeoutMs timeout, bool peek = true);
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
	using _Base = net::TCPSocket;
	std::mutex _sendMutex;
	std::timed_mutex _recvMutex;
};
} // namespace arch::net::async
