#pragma once

namespace arch::net {

struct Init final {
	static void init();
	static void cleanup();
};

} // namespace arch::net
