#pragma once

#ifdef _WIN32
// OS is Windows
#elif defined unix
// OS is linux
#endif

#include <net/ipv4.hpp>
#include <net/ipv4_network.hpp>
#include <net/host.hpp>
#include <net/__net_init.hpp>

namespace arch {
	/// @brief Namespace of archimedes' networking module.
	///
	namespace net {
	}
}