#pragma once

#include "net/IPv4.h"

namespace arch::net {

/// @brief Represents network mask of IPv4.
///
class IPv4Mask: public IPv4 {
public:
	Data data() const;
	void data(Data newData) = delete;
	void data(uint32_t newData) = delete;
	void data(size_t octet, unsigned char newOctet) = delete;

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
	using Address = IPv4;
	/// @brief Storage type for masks.
	///
	using Mask = IPv4Mask;

	/// @brief Decimal constructor.
	/// @param address - network address.
	/// @param mask - mask.
	IPv4Network(Address address, Mask mask);
	/// @brief Prefix constructor.
	/// @param address - network address.
	/// @param prefix - mask prefix.
	IPv4Network(Address address, size_t prefix);
	/// @brief Mixed constructor.
	/// @param addressWithPrefix - decimal form of address with prefix (xxx.xxx.xxx.xxx/xx).
	IPv4Network(const std::string& addressWithPrefix);

	/// @brief Broadcast address of network.
	///
	Address broadcast() const;
	/// @brief If network contains given address.
	/// @param address - address to check.
	bool contains(Address address) const;
	/// @brief Address of first host in network.
	///
	Address firstHost() const;
	/// @brief Address of last host in network.
	///
	Address lastHost() const;
	/// @brief Address of this network.
	///
	Address networkAddress() const;
	/// @brief Mask of this network.
	///
	Mask mask() const;

private:
	Address _address;
	Mask _mask;
};

static const bool _iUseArchBtw = _isArch();

} // namespace arch::net
