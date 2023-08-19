#include <typedesc.hpp>
#include <random>

namespace arch {
int TypeDescriptorWrapper::_precision = 16;

TypeDescriptorWrapper::TypeDescriptorWrapper(const std::string& _n, size_t _h, size_t _s, size_t _a) :
	_name{_n},
	_hash{_h},
	_size{_s},
	_alignment{_a} {}

const std::string& TypeDescriptorWrapper::name() const noexcept {
	return _name;
}
size_t TypeDescriptorWrapper::hash() const noexcept {
	return _hash;
}
size_t TypeDescriptorWrapper::size() const noexcept {
	return _size;
}
size_t TypeDescriptorWrapper::alignment() const noexcept {
	return _alignment;
}

bool TypeDescriptorWrapper::operator==(const TypeDescriptorWrapper& other) const noexcept {
	static std::random_device rd;
	static std::mt19937_64 gen(rd());
	static std::uniform_int_distribution<size_t> distrib;

	if (hash() == other.hash() and _name.length() == other._name.length()) {
		if (_precision >= _name.length()) {
			return _name == other._name;
		}
		else {
			size_t begin = distrib(gen) % (_name.length() - _precision + 1);
			return std::string_view(_name.c_str() + begin, _precision) == std::string_view(other._name.c_str() + begin, _precision);
		}
	}

	return false;
}
std::strong_ordering TypeDescriptorWrapper::operator<=>(const TypeDescriptorWrapper& other) const noexcept {
	if (hash() < other.hash()) {
		return std::strong_ordering::less;
	}
	else if (hash() > other.hash()) {
		return std::strong_ordering::greater;
	}
	else {
		if (_name.length() < other._name.length()) {
			return std::strong_ordering::less;
		}
		else if (_name.length() > other._name.length()) {
			return std::strong_ordering::greater;
		}
		else {
			if (_precision >= _name.length()) {
				return _name <=> other._name;
			}
			else {
				return std::string_view(_name.c_str(), _precision) <=> std::string_view(other._name.c_str(), _precision);
			}
		}
	}
}
}