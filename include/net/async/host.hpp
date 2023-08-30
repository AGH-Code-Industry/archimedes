#pragma once

#include <net/utilities.hpp>
#include <net/ipv4.hpp>
#include <vector>
#include <future>

namespace arch::net {
class Host;
}

namespace arch::net::async {
/// @brief Represents asynchronous host in IPv4 network.
///
class Host {
public:
	/// @brief result of update() method
	///
	enum class update_result {
		success,
		failure,
		timeout,
		none
	};

	/// @brief IPv4 constructor.
	/// @param ip - IPv4 of host.
	Host(IPv4 ip);
	/// @brief Copy constructor.
	///
	Host(const Host& other);

	/// @brief Returns localhost
	/// @details Loopback address is guaranteed to be returned by ip().
	///
	static Host localhost();
	/// @brief Returns localhost asynchronously
	/// @details Loopback address is guaranteed to be returned by ip().
	/// @param update - if to call update afterwards
	/// @param timeout - timeout in milliseconds 
	static std::future<std::pair<Host, update_result>> localhost(bool update, timeout_t timeout = 3000);
	/// @brief Creates Host from given IPv4 and updates it asynchronously.
	/// @param ip - IPv4 of host.
	/// @param update - if to call update afterwards
	/// @param timeout - timeout in milliseconds
	/// @return future object containing Host.
	static std::future<std::pair<Host, update_result>> from_ip(IPv4 ip, bool update = false, timeout_t timeout = 3000);
	/// @brief Creates Host from given hostname and updates it asynchronously.
	/// @param hostname - hostname.
	/// @param timeout - timeout in milliseconds
	/// @return future object containing Host.
	static std::future<std::pair<Host, update_result>> from_hostname(std::string_view hostname, timeout_t timeout = 3000);

	/// @brief Gets first IPv4 of host.
	///
	const IPv4& ip() const;
	/// @brief Gets IPv4 addresses of host.
	///
	const std::vector<IPv4>& ips() const;
	/// @brief Checks if host has given address in its list.
	/// @param address - address to check
	bool has_ip(IPv4 address) const;
	/// @brief Gets hostname of host.
	///
	const std::string& hostname() const;
	/// @brief Updates host info asynchronously.
	/// @param timeout - timeout in milliseconds
	/// @return future object containing result of operation.
	std::future<update_result> update(timeout_t timeout = 3000);

	/// @brief Returns synchronous version of this host.
	/// 
	net::Host sync() const;

private:
	friend class net::Host;

	Host();

	std::string _update_hostname();

	std::vector<IPv4> _ips{IPv4()};
	std::string _hostname;

	std::timed_mutex _update_mutex;
};
}