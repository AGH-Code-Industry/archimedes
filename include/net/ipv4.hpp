#pragma once

#include <compare>
#include <iostream>
#include <net/utilities.hpp>

namespace arch::net {
/// @brief Represents IPv4 address.
///
class IPv4 {
public:
	/// @brief Type of octet.
	///
	using octet_type = unsigned char;
	/// @brief Type of binary value.
	///
	using binary_type = uint32_t;
	/// @brief Union type of data
	///
	union data_type {
		octet_type octets[4];
		binary_type binary;
	};

	/// @brief Example loopback address.
	///
	static IPv4 localhost;

	/// @brief Default constructor.
	///
	IPv4();
	/// @brief Decimal value constructor.
	/// @param decimal - decimal form of IPv4 address.
	IPv4(std::string_view decimal);
	/// @brief Octet array constructor.
	/// @param octets - array of 4 octets of IPv4 address.
	IPv4(octet_type octets[4]);
	/// @brief Octets constructor.
	/// @param octet_0 - first octet.
	/// @param octet_0 - second octet.
	/// @param octet_0 - third octet.
	/// @param octet_0 - forth octet.
	IPv4(octet_type octet_0, octet_type octet_1, octet_type octet_2,
		 octet_type octet_3);
	/// @brief Binary value constructor
	/// @param binary - binary value of address.
	explicit IPv4(binary_type binary);
	/// @brief in_addr constructor.
	/// @param addr - in_addr structure to convert from.
	IPv4(in_addr addr);

	/// @brief Gets decimal form of address.
	///
	std::string str() const;
	/// @brief Sets decimal value of address.
	/// @param decimal - decimal form of IPv4 address.
	void str(std::string_view decimal);
	/// @brief Converts to in_addr.
	///
	operator in_addr() const;

	/// @brief Obtains data of address.
	///
	const data_type& data() const;
	/// @brief Sets data of address.
	/// @param new_data - new data of address.
	void data(data_type new_data);
	/// @brief Sets binary data of address.
	/// @param new_data - new data of address.
	void data(binary_type new_data);
	/// @brief Sets octet data of address.
	/// @param octet - number of octet to set.
	/// @param new_octet - new value of octet.
	void data(size_t octet, octet_type new_octet);

	/// @brief Preincrement operator.
	///
	IPv4& operator++();
	/// @brief Postincrement operator.
	///
	IPv4 operator++(int);
	/// @brief Predecrement operator.
	///
	IPv4& operator--();
	/// @brief Postdecrement operator.
	///
	IPv4 operator--(int);

	/// @brief Equality operator
	/// @param second - IPv4 to compare with.
	/// @return True if equal, false otherwise.
	bool operator==(IPv4 second) const;
	/// @brief Three-way comparision operator.
	/// @param second - IPv4 to compare with.
	std::strong_ordering operator<=>(IPv4 second) const;

protected:
	data_type _data;
};

/// @brief ostream insertion operator.
/// @param os - ostream.
/// @param ip - ip to insert.
///
std::ostream& operator<<(std::ostream& os, const IPv4& ip);
}  // namespace arch::net