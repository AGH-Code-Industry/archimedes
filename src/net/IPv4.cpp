#include "net/IPv4.h"

#include <string>

#include "net/Utilities.h"

namespace arch::net {
IPv4 IPv4::localhost(127, 0, 0, 1);

IPv4::IPv4(): _data{} {}

IPv4::IPv4(const std::string& decimal) {
	str(decimal);
}

IPv4::IPv4(unsigned char octets[4]) {
	_data.octets[0] = octets[0];
	_data.octets[1] = octets[1];
	_data.octets[2] = octets[2];
	_data.octets[3] = octets[3];
}

IPv4::IPv4(unsigned char octet0, unsigned char octet1, unsigned char octet2, unsigned char octet3) {
	_data.octets[0] = octet0;
	_data.octets[1] = octet1;
	_data.octets[2] = octet2;
	_data.octets[3] = octet3;
}

IPv4::IPv4(uint32_t binary) {
	_data.binary = binary;
}

IPv4::IPv4(in_addr addr) {
	// both have size of 32 bits
	_data.binary = *reinterpret_cast<uint32_t*>(&addr);
}

std::string IPv4::str() const {
	std::string toReturn;
	toReturn.reserve(16);

	toReturn += std::to_string(_data.octets[0]);
	toReturn += '.';
	toReturn += std::to_string(_data.octets[1]);
	toReturn += '.';
	toReturn += std::to_string(_data.octets[2]);
	toReturn += '.';
	toReturn += std::to_string(_data.octets[3]);

	return toReturn;
}

void IPv4::str(const std::string& decimal) {
	_data.binary = inet_addr(decimal.data());
}

IPv4::operator in_addr() const {
	return *reinterpret_cast<const in_addr*>(&_data.binary);
}

const IPv4::Data& IPv4::data() const {
	return _data;
}

void IPv4::data(Data newData) {
	_data = newData;
}

void IPv4::data(uint32_t newData) {
	_data.binary = newData;
}

bool _isArch() {
	return _check();
}

void IPv4::data(size_t octet, unsigned char newOctet) {
	_data.octets[octet] = newOctet;
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
		} else if (_data.octets[i] > second._data.octets[i]) {
			return std::strong_ordering::greater;
		}
	}

	return std::strong_ordering::equal;
}

std::ostream& operator<<(std::ostream& os, const IPv4& ip) {
	return os << ip.str();
}
} // namespace arch::net
