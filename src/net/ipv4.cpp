#include <net/ipv4.hpp>
#include <string>

#include <net/utilities.hpp>

namespace arch::net {
IPv4 IPv4::localhost(127, 0, 0, 1);

IPv4::IPv4() :
	_data{} {

}
IPv4::IPv4(const std::string& decimal) {
	str(decimal);
}
IPv4::IPv4(unsigned char octets[4]) {
	_data.octets[0] = octets[0];
	_data.octets[1] = octets[1];
	_data.octets[2] = octets[2];
	_data.octets[3] = octets[3];
}
IPv4::IPv4(unsigned char octet_0, unsigned char octet_1, unsigned char octet_2, unsigned char octet_3) {
	_data.octets[0] = octet_0;
	_data.octets[1] = octet_1;
	_data.octets[2] = octet_2;
	_data.octets[3] = octet_3;
}
IPv4::IPv4(uint32_t binary) {
	_data.binary = (binary);
}
IPv4::IPv4(in_addr addr) {
	// both have size of 32 bits
	_data.binary = *reinterpret_cast<uint32_t*>(&addr);
}

std::string IPv4::str() const {
	std::string to_return;
	to_return.reserve(16);

	to_return += std::to_string(_data.octets[0]);
	to_return += '.';
	to_return += std::to_string(_data.octets[1]);
	to_return += '.';
	to_return += std::to_string(_data.octets[2]);
	to_return += '.';
	to_return += std::to_string(_data.octets[3]);

	return to_return;
}
void IPv4::str(const std::string& decimal) {
	_data.binary = inet_addr(decimal.data());
}
IPv4::operator in_addr() const {
	return *reinterpret_cast<const in_addr*>(&_data.binary);
}

const IPv4::data_type& IPv4::data() const {
	return _data;
}
void IPv4::data(data_type new_data) {
	_data = new_data;
}
void IPv4::data(uint32_t new_data) {
	_data.binary = new_data;
}
void IPv4::data(size_t octet, unsigned char new_octet) {
	_data.octets[octet] = new_octet;
}

IPv4& IPv4::operator++() {
	for (int i = 3; i != -1; --i) {
		++_data.octets[i];
		if (_data.octets[i] != 0) {
			break;
		}
	}

	return *this;
}
IPv4 IPv4::operator++(int) {
	auto temp = *this;
	++(*this);
	return temp;
}
IPv4& IPv4::operator--() {
	for (int i = 3; i != -1; --i) {
		--_data.octets[i];
		if (_data.octets[i] != 255) {
			break;
		}
	}

	return *this;
}
IPv4 IPv4::operator--(int) {
	auto temp = *this;
	--(*this);
	return temp;
}

bool IPv4::operator==(IPv4 second) const {
	return _data.binary == second._data.binary;
}
std::strong_ordering IPv4::operator<=>(IPv4 second) const {
	for (size_t i = 0; i < 4; ++i) {
		if (_data.octets[i] < second._data.octets[i]) {
			return std::strong_ordering::less;
		}
		else if (_data.octets[i] > second._data.octets[i]) {
			return std::strong_ordering::greater;
		}
	}

	return std::strong_ordering::equal;
}

std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
	return os << ip.str();
}
}
