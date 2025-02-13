#pragma once

#include <string>

namespace arch::scene {

struct NameComponent: public std::string {
	using std::string::string;
	using std::string::operator=;
};

} // namespace arch::scene
