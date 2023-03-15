#pragma once

#include <iostream>
#include <array>
#include <WinSock2.h>

namespace net
{
	/// @brief Represents IPv4 address.
	///
	class IPv4
	{
	public:
		/// @brief Type of octet.
		///
		using octet_type = unsigned char;
		/// @brief Type of binary value.
		///
		using binary_type = uint32_t;
		/// @brief Union type of data
		///
		union data_type
		{
			octet_type octets[ 4 ];
			binary_type binary;
		};

		data_type data;

		/// @brief Default constructor.
		///
		IPv4();
		/// @brief Decimal value constructor.
		/// @param decimal - decimal form of IPv4 address.
		IPv4(std::string_view decimal);
		/// @brief Octet array constructor.
		/// @param octets - array of 4 octets of IPv4 address.
		IPv4(octet_type octets[ 4 ]);
		/// @brief Octets constructor.
		/// @param octet_0 - first octet.
		/// @param octet_0 - second octet.
		/// @param octet_0 - third octet.
		/// @param octet_0 - forth octet.
		IPv4(octet_type octet_0, octet_type octet_1, octet_type octet_2, octet_type octet_3);
		/// @brief Binary value constructor
		/// @param binary - binary value of address.
		IPv4(binary_type binary);
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
	};

	/// @brief ostream insertion operator.
	/// @param os - ostream.
	/// @param ip - ip to insert.
	///
	std::ostream& operator<<(std::ostream& os, const IPv4& ip);
}