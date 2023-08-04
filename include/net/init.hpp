#pragma once

namespace arch::net {
class Init final {
public:
	static void init();
	static void cleanup();
	static bool initialized();
private:
	inline static bool _initialized = false;
};
}