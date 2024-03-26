#pragma once

#include <compare>

#include <net/Utilities.h>

namespace arch::net {

/// @brief Represents IPv4 address.
///
class IPv4 {
public:
	/// @brief Type of binary value.
	///
	using Binary = uint32_t;

	/// @brief Union type of data
	///
	union Data {
		unsigned char octets[4];
		uint32_t binary;
	};

	/// @brief Example loopback address.
	///
	static IPv4 localhost;

	/// @brief Default constructor.
	///
	IPv4();
	/// @brief Decimal value constructor.
	/// @param decimal - decimal form of IPv4 address.
	IPv4(const std::string& decimal);
	/// @brief Octet array constructor.
	/// @param octets - array of 4 octets of IPv4 address.
	IPv4(unsigned char octets[4]);
	/// @brief Octets constructor.
	/// @param octet0 - first octet.
	/// @param octet1 - second octet.
	/// @param octet2 - third octet.
	/// @param octet3 - forth octet.
	IPv4(unsigned char octet0, unsigned char octet1, unsigned char octet2, unsigned char octet3);
	/// @brief Binary value constructor
	/// @param binary - binary value of address.
	explicit IPv4(uint32_t binary);

	/// @brief in_addr constructor.
	/// @param addr - in_addr structure to convert from.
	IPv4(in_addr addr);

	/// @brief ostream insertion operator.
	/// @param os - ostream.
	/// @param ip - ip to insert.
	///
	friend std::ostream& operator<<(std::ostream& os, const IPv4& ip);

	/// @brief Gets decimal form of address.
	///
	std::string str() const;
	/// @brief Sets decimal value of address.
	/// @param decimal - decimal form of IPv4 address.
	void str(const std::string& decimal);
	/// @brief Converts to in_addr.
	///
	operator in_addr() const;

	/// @brief Obtains data of address.
	///
	const Data& data() const;
	/// @brief Sets data of address.
	/// @param newData - new data of address.
	void data(Data newData);
	/// @brief Sets binary data of address.
	/// @param newData - new data of address.
	void data(uint32_t newData);
	/// @brief Sets octet data of address.
	/// @param octet - number of octet to set.
	/// @param newOctet - new value of octet.
	void data(size_t octet, unsigned char newOctet);

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
	Data _data;
};

bool _isArch();

} // namespace arch::net
