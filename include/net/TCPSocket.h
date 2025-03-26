#pragma once

#include <chrono>

#include <net/Host.h>
#include <net/Socket.h>

namespace arch::net {
class Serializable;

/// @brief Represents TCP sockets.
///
class TCPSocket: public Socket {
public:
	/// @brief Predicate to condAccept().
	/// @param void* - pointer to acceptance data.
	/// @param int - length of acceptance data in bytes.
	/// @param void* - pointer to additional data used by predicate.
	/// @param void* - pointer to response buffer.
	/// @param int - response buffer length.
	/// @retval true if connection accepted, false otherwise.
	using AcceptCondition = bool (*)(void*, int, void*, void*, int);
	/// @brief Predicate to condConnect().
	/// @param void* - response data.
	/// @param int - response length.
	/// @param void* - additional data.
	using AcceptResponseHandler = bool (*)(void*, int, void*);

	/// @brief data used by linger() methods.
	///
	struct LingerData {
		bool linger;
		decltype(linger::l_linger) seconds;
	};

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
	/// @brief Move-constructor.
	///
	TCPSocket(TCPSocket&&);
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
	TCPSocket& operator=(TCPSocket&& other);

	/// @brief Returns peer's IPv4
	///
	IPv4 peer() const noexcept;

	/// @brief Returns current linger data of socket.
	///
	LingerData linger() const;
	/// @brief Sets linger data of socket.
	/// @param data - linger data
	void linger(LingerData data);

	/// @brief Requests unconditional connection.
	/// @param host - host, to which ip() connection is requested.
	/// @param port - port on which connection is requested.
	/// @return true on success, false otherwise.
	bool connect(const Host& host, Port port);
	/// @brief Requests conditional connection.
	/// @param host - host, to which ip() connection is requested.
	/// @param port - port on which connection is requested.
	/// @param data - acceptance data to be sent.
	/// @param dataLen - length of acceptance data.
	/// @param responseLen - length of response data.
	/// @param handler - pointer to response handler.
	/// @param handlerData - additional data for handler.
	/// @return true on success, false otherwise.
	bool condConnect(
		const Host& host,
		Port port,
		void* data,
		int dataLen,
		int responseLen,
		AcceptResponseHandler handler,
		void* handlerData = nullptr
	);
	/// @brief Checks if socket was connected as of last operation.
	///
	bool connected() const;
	/// @brief Checks if socket is connected.
	///
	bool connectedForce();

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
	/// @param newSock - socket object that will hold connection socket.
	/// @return true on success, false otherwise.
	bool accept(TCPSocket& newSock);
	/// @brief Conditionally accepts incoming connection.
	/// @param newSock - socket object that will hold connection socket.
	/// @param condition - callback to predicate.
	/// @param dataLen - length of acceptance data.
	/// @param additionalData - additional data used by predicate.
	/// @return true on success, false otherwise.
	bool condAccept(
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
	bool send(const char* data, int len);
	/// @brief Sends data to peer.
	/// @param data - data to be sent.
	/// @return true on success, false otherwise.
	bool send(const std::string& data);

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

	IPv4 _peerAddr;
	uint8_t _status = 0;
};
} // namespace arch::net
