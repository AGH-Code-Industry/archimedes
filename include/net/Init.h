#pragma once

namespace arch::net {

struct Init final {
	/// @brief Initializes networking
	static void init();
	/// @brief Networking cleanup
	static void cleanup();
};

} // namespace arch::net
