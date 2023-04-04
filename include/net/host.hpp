#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#endif
#ifdef linux
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#endif
#include <net/ipv4.hpp>
#include <vector>

namespace arch::net {
	/// @brief Represents host in IPv4 network.
	///
	class Host {
	public:
		/// @brief IPv4 constructor.
		/// @param ip - IPv4 of host.
		/// @param get_hostname - if should search for hostname.
		Host(IPv4 ip, bool get_hostname = false);
		/// @brief Hostname constructor.
		/// @details In difference to IP constructor, this searches in network for specified host. hostname() will remain empty if not found.
		/// @param hostname - Hostname of host.
		Host(std::string_view hostname);

		/// @brief Returns localhost
		///
		static Host localhost();

		/// @brief Gets first IPv4 of host.
		///
		const IPv4& ip() const;
		/// @brief Gets IPv4 addresses of host.
		///
		const std::vector<IPv4>& ips() const;
		/// @brief Gets hostname of host.
		///
		const std::string& hostname() const;
		/// @brief Searches for hostname and updates it.
		///
		void update_hostname();

	private:
		std::vector<IPv4> _ips{IPv4()};
		std::string _hostname;
	};
}