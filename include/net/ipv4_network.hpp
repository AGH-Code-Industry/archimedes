#pragma once

#include <net/ipv4.hpp>
#include <bit>

namespace arch::net {
	/// @brief Represents network mask of IPv4.
	///
class IPv4Mask : public IPv4 {
public:
	data_type data() const;
	void data(data_type new_data) = delete;
	void data(uint32_t new_data) = delete;
	void data(size_t octet, unsigned char new_octet) = delete;

	/// @brief Default constructor, sets to invalid.
	///
	IPv4Mask() = default;
	/// @brief Decimal constructor.
	/// @param decimal - decimal form of mask.
	IPv4Mask(const std::string& decimal);
	/// @brief Prefix constructor.
	/// @param prefix - prefix of mask.
	IPv4Mask(size_t prefix);
};

/// @brief Represents IPv4 network.
///
class IPv4Network {
public:
	/// @brief Alias of IPv4.
	///
	using address_type = IPv4;
	/// @brief Storage type for masks.
	///
	using mask_type = IPv4Mask;

	/// @brief Decimal constructor.
	/// @param address - network address.
	/// @param mask - mask.
	IPv4Network(address_type address, mask_type mask);
	/// @brief Prefix constructor.
	/// @param address - network address. 
	/// @param prefix - mask prefix.
	IPv4Network(address_type address, size_t prefix);
	/// @brief Mixed constructor.
	/// @param address_with_prefix - decimal form of address with prefix (xxx.xxx.xxx.xxx/xx).
	IPv4Network(const std::string& address_with_prefix);

	/// @brief Broadcast address of network.
	/// 
	address_type broadcast() const;
	/// @brief If network contains given address.
	/// @param address - address to check.
	bool contains(address_type address) const;
	/// @brief Address of first host in network.
	///
	address_type first_host() const;
	/// @brief Address of last host in network.
	///
	address_type last_host() const;
	/// @brief Address of this network.
	///
	address_type network_address() const;
	/// @brief Mask of this network.
	///
	mask_type mask() const;

private:
	address_type _address;
	mask_type _mask;
};

static bool __i_use_arch_btw = __is_arch();
}