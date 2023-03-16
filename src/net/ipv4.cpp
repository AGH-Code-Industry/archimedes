#include <net/ipv4.hpp>
#include <string>

#pragma comment(lib, "ws2_32.lib")

namespace net
{
	IPv4 IPv4::localhost(127, 0, 0, 1);

	IPv4::IPv4() :
		data{}
	{

	}
	IPv4::IPv4(std::string_view decimal)
	{
		str(decimal);
	}
	IPv4::IPv4(octet_type octets[ 4 ])
	{
		data.octets[ 0 ] = octets[ 0 ];
		data.octets[ 1 ] = octets[ 1 ];
		data.octets[ 2 ] = octets[ 2 ];
		data.octets[ 3 ] = octets[ 3 ];
	}
	IPv4::IPv4(octet_type octet_0, octet_type octet_1, octet_type octet_2, octet_type octet_3)
	{
		data.octets[ 0 ] = octet_0;
		data.octets[ 1 ] = octet_1;
		data.octets[ 2 ] = octet_2;
		data.octets[ 3 ] = octet_3;
	}
	IPv4::IPv4(binary_type binary)
	{
		data.binary = binary;
	}
	IPv4::IPv4(in_addr addr)
	{
		// both have size of 32 bits
		data.binary = *reinterpret_cast<binary_type*>( &addr );
	}

	std::string IPv4::str() const
	{
		std::string to_return;
		to_return.reserve(16);

		to_return += std::to_string(data.octets[ 0 ]);
		to_return += '.';
		to_return += std::to_string(data.octets[ 1 ]);
		to_return += '.';
		to_return += std::to_string(data.octets[ 2 ]);
		to_return += '.';
		to_return += std::to_string(data.octets[ 3 ]);

		return to_return;
	}
	void IPv4::str(std::string_view decimal)
	{
		data.binary = inet_addr(decimal.data());
	}
	IPv4::operator in_addr() const
	{
		return *reinterpret_cast<const in_addr*>( &data.binary );
	}

	std::ostream& operator<<(std::ostream& os, const IPv4& ip)
	{
		return os << ip.str();
	}
}