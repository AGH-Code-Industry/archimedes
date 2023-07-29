#ifdef _WIN32
#define NOMINMAX
#endif
#include <net/ipv4_network.hpp>
#include <vector>
#include <string>

namespace arch::net {
IPv4Mask IPv4Mask::invalid(32);

IPv4Mask::IPv4Mask() {
	*this = invalid;
}
IPv4Mask::IPv4Mask(std::string_view decimal) :
	IPv4(decimal) {
	for (size_t i = 0; i != 4; ++i) {
		if (i != 0 and _data.octets[i - 1] < _data.octets[i]) {
			*this = invalid;
			return;
		}
		switch (_data.octets[i]) {
			default:
				*this = invalid;
				return;
			case octet_type(255 << 0):
			case octet_type(255 << 1):
			case octet_type(255 << 2):
			case octet_type(255 << 3):
			case octet_type(255 << 4):
			case octet_type(255 << 5):
			case octet_type(255 << 6):
			case octet_type(255 << 7):
			case octet_type(255 << 8):
				break;
		}
	}
}
IPv4Mask::IPv4Mask(size_t prefix) {
	if (prefix == 0) {
		_data.binary = 0;
		return;
	}
	_data.binary = (0xffffffff << (32 - prefix));
	auto temp = _data.octets[0];
	_data.octets[0] = _data.octets[3];
	_data.octets[3] = temp;
	temp = _data.octets[1];
	_data.octets[1] = _data.octets[2];
	_data.octets[2] = temp;
}
IPv4Mask::data_type IPv4Mask::data() const {
	return _data;
}

IPv4Network::IPv4Network(address_type address, mask_type mask) :
	_address{address.data().binary & mask.data().binary},
	_mask{mask} {

}
IPv4Network::IPv4Network(address_type address, size_t prefix) :
	IPv4Network(address, IPv4Mask(prefix)) {

}
IPv4Network::IPv4Network(std::string_view address_with_prefix) {
	auto divisor = address_with_prefix.find('/');
	std::string mask{address_with_prefix.substr(divisor + 1)};
	std::string address{address_with_prefix.substr(0, divisor)};

	*this = IPv4Network(IPv4(address), IPv4Mask(std::stoul(mask)));
}

IPv4Network::address_type IPv4Network::broadcast() const {
	return IPv4((compl _mask.data().binary) bitor _address.data().binary);
}
bool IPv4Network::contains(address_type address) const {
	return first_host() <= address and address <= last_host();
}
IPv4Network::address_type IPv4Network::first_host() const {
	auto temp = _address;
	return ++temp;
}
IPv4Network::address_type IPv4Network::last_host() const {
	auto temp = broadcast();
	return --temp;
}
IPv4Network::address_type IPv4Network::network_address() const {
	return _address;
}
IPv4Network::mask_type IPv4Network::mask() const {
	return _mask;
}
}