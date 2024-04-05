#include "net/IPv4Network.h"

#include <string>
#include <vector>

#include "net/IPv4.h"
#include "net/NetException.h"
#include "net/Utilities.h"

namespace arch::net {

IPv4Mask::IPv4Mask(const std::string& decimal): IPv4(decimal) {
	bool onlyOnes = true;
	for (auto&& octet : _data.octets) {
		for (int i = 7; i != -1; --i) {
			if (onlyOnes and ((octet & (1 << i)) >> i) == 0) {
				onlyOnes = false;
			} else if (not onlyOnes and ((octet & (1 << i)) >> i) == 1) {
				throw NetException("invalid mask");
			}
		}
	}
}

IPv4Mask::IPv4Mask(size_t prefix) {
	if (prefix == 0) {
		_data.binary = 0;
		return;
	}
	_data.binary = (0xffff'ffff << (32 - prefix));
	std::swap(_data.octets[0], _data.octets[3]);
	std::swap(_data.octets[1], _data.octets[2]);
}

IPv4Mask::Data IPv4Mask::data() const {
	return _data;
}

IPv4Network::IPv4Network(Address address, Mask mask):
	_address{address.data().binary & mask.data().binary},
	_mask{mask} {}

IPv4Network::IPv4Network(Address address, size_t prefix): IPv4Network(address, IPv4Mask(prefix)) {}

IPv4Network::IPv4Network(const std::string& addressWithPrefix) {
	auto divisor = addressWithPrefix.find('/');
	std::string mask{addressWithPrefix.substr(divisor + 1)};
	std::string address{addressWithPrefix.substr(0, divisor)};

	*this = IPv4Network(IPv4(address), IPv4Mask(std::stoul(mask)));
}

IPv4Network::Address IPv4Network::broadcast() const {
	return IPv4((compl _mask.data().binary) bitor _address.data().binary);
}

bool IPv4Network::contains(Address address) const {
	return firstHost() <= address and address <= lastHost();
}

IPv4Network::Address IPv4Network::firstHost() const {
	auto temp = _address;
	return ++temp;
}

IPv4Network::Address IPv4Network::lastHost() const {
	auto temp = broadcast();
	return --temp;
}

IPv4Network::Address IPv4Network::networkAddress() const {
	return _address;
}

IPv4Network::Mask IPv4Network::mask() const {
	return _mask;
}

} // namespace arch::net
