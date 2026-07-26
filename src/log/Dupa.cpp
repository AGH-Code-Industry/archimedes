#include <archimedes/log/Dupa.h>

namespace arch {

static inline std::atomic<u32> dupaVal = 0;

u32 dupa(log::Level level) noexcept {
	auto val = dupaVal.fetch_add(1, std::memory_order::relaxed);
	log::_details::logImpl(level, 2, "dupa {}", val);
	return val;
}

u32 dupa(const u32 newVal, log::Level level) noexcept {
	dupaVal.store(newVal + 1, std::memory_order::relaxed);
	log::_details::logImpl(level, 2, "dupa {}", newVal);
	return newVal;
}

} // namespace arch
