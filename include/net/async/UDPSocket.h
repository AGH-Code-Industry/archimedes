#pragma once

#include "net/UDPSocket.h"
#include "net/async/Host.h"

namespace arch::net::async {

/// @brief Class representing asynchronous UDPSocket.
///
class UDPSocket: protected net::UDPSocket {
public:
	/// @brief Default constructor.
	///
	UDPSocket();
	/// @brief Any-binding constructor
	/// @details Binds to all interfaces using INADDR_ANY.
	UDPSocket(Port port);
	/// @brief Binding constructor.
	///
	UDPSocket(IPv4 address, Port port);
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
	using net::UDPSocket::broadcastEnabled;
	using net::UDPSocket::close;
	using net::UDPSocket::dataAvalible;
#if ARCHIMEDES_WINDOWS
	using net::UDPSocket::exclusive;
#endif
	using net::UDPSocket::port;
	using net::UDPSocket::protocol;
	using net::UDPSocket::recvBuf;
	using net::UDPSocket::reuse;
	using net::UDPSocket::sendable;
	using net::UDPSocket::sendBuf;
	using net::UDPSocket::usable;
	using net::UDPSocket::UsableData;

	/// @brief Asynchronously sends given data to given host.
	/// @details If data length exceeds sendBuff(), error occurs.
	/// @param host - host to send data to, ip() will be used as address.
	/// @param port - port to send data to.
	/// @param data - data to be sent.
	/// @param length - length of data to be sent.
	/// @return std::future with info if succeeded.
	std::future<bool> sendTo(const Host& host, Port port, const char* data, int length);
	/// @brief Sends given data to given host.
	/// @details If data length exceeds sendBuff(), error occurs.
	/// @param host - host to send data to, ip() will be used as address.
	/// @param data - data to be sent.
	/// @param length - length of data to be sent.
	/// @return std::future with info if succeeded.
	std::future<bool> sendTo(const Host& host, const char* data, int length);
	/// @brief Sends given data to given host.
	/// @details If data length exceeds sendBuff(), error occurs.
	/// @param host - host to send data to, ip() will be used as address.
	/// @param port - port to send data to.
	/// @param data - data to be sent.
	/// @return std::future with info if succeeded.
	std::future<bool> sendTo(const Host& host, Port port, const std::string& data);
	/// @brief Sends given data to given host.
	/// @details If data length exceeds sendBuff(), error occurs.
	/// @param host - host to send data to, ip() will be used as address.
	/// @param data - data to be sent.
	/// @return std::future with info if succeeded.
	std::future<bool> sendTo(const Host& host, const std::string& data);

	/// @brief Asynchronously receives data.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param length - reference to value to save length of data to.
	/// @param timeout - timeout, if negative wait indefinetly
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	std::future<bool> recv(char* buf, int buflen, int& length, TimeoutMs timeout, bool peek = false);
	/// @brief Asynchronously receives data, waits indefinetly.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param length - reference to value to save length of data to.
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	std::future<bool> recv(char* buf, int buflen, int& length, bool peek = false);
	/// @brief Asynchronously receives data.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param timeout - timeout, if negative wait indefinetly
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	std::future<bool> recv(char* buf, int buflen, TimeoutMs timeout, bool peek = false);
	/// @brief Asynchronously receives data, waits indefinetly.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return true if received data, false otherwise.
	std::future<bool> recv(char* buf, int buflen, bool peek = false);
	/// @brief Asynchronously receives data and sender.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param length - reference to value to save length of data to.
	/// @param timeout - timeout, if negative wait indefinetly
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return Unupdated sender host on success, Host("0.0.0.0") otherwise.
	std::future<Host> recvFrom(char* buf, int buflen, int& length, TimeoutMs timeout, bool peek = false);
	/// @brief Asynchronously receives data and sender, waits indefinetly.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param length - reference to value to save length of data to.
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return Unupdated sender host on success, Host("0.0.0.0") otherwise.
	std::future<Host> recvFrom(char* buf, int buflen, int& length, bool peek = false);
	/// @brief Asynchronously receives data and sender.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param timeout - timeout, if negative wait indefinetly
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return Unupdated sender host on success, Host("0.0.0.0") otherwise.
	std::future<Host> recvFrom(char* buf, int buflen, TimeoutMs timeout, bool peek = false);
	/// @brief Asynchronously receives data and sender, waits indefinetly.
	/// @param buf - buffer to save data to.
	/// @param buflen - length of buffer.
	/// @param peek - if to copy data but not erase it from socket's buffer (false by default).
	/// @return Unupdated sender host on success, Host("0.0.0.0") otherwise.
	std::future<Host> recvFrom(char* buf, int buflen, bool peek = false);

private:
	using _Base = net::UDPSocket;
	std::mutex _sendMutex;
	std::timed_mutex _recvMutex;
};

} // namespace arch::net::async
