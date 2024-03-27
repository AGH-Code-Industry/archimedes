#pragma once

#include <net/Utilities.h>
#include <net/IPv4.h>
#include <vector>

namespace arch::net {
	namespace async {
		class Host;
	}

	/// @brief Represents host in IPv4 network.
	///
class Host {
public:
	/// @brief IPv4 constructor.
	/// @param ip - IPv4 of host.
	/// @param update - if should update via the network
	Host(IPv4 ip, bool update = false);
	/// @brief Hostname constructor.
	/// @details In difference to IP constructor, this searches in network for specified host. hostname() will remain empty if not found.
	/// @param hostname - Hostname of host.
	Host(const std::string& hostname);

	/// @brief Returns localhost
	/// @details Loopback address is guaranteed to be returned by ip().
	/// @param update - if should update via the network
	static Host localhost(bool update = false);

	/// @brief Gets first IPv4 of host.
	///
	const IPv4& ip() const;
	/// @brief Gets IPv4 addresses of host.
	///
	const std::vector<IPv4>& ips() const;
	/// @brief Checks if host has given address in its list.
	/// @param address - address to check
	bool hasIp(IPv4 address) const;
	/// @brief Gets hostname of host.
	///
	const std::string& hostname() const;
	/// @brief Updates host info.
	/// @return true on success, false otherwise.
	bool update();

	/// @brief Returns asynchronous version of this host.
	///
	async::Host async() const;

private:
	friend class async::Host;

	Host();

	std::string _updateHostname();

	std::vector<IPv4> _ips{IPv4()};
	std::string _hostname;
};
}