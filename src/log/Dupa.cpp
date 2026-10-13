#include <archimedes/log/Dupa.h>

namespace arch {

static inline std::atomic<u32> dupaVal = 0;

u32 dupa(log::Level level, const utils::SimpleSourceLocation loc) noexcept {
	auto val = dupaVal.fetch_add(1, std::memory_order::relaxed);
	log::_details::logImpl(level, loc, "dupa {}", val);
	return val;
}

u32 dupa(const u32 newVal, log::Level level, const utils::SimpleSourceLocation loc) noexcept {
	dupaVal.store(newVal + 1, std::memory_order::relaxed);
	log::_details::logImpl(level, loc, "dupa {}", newVal);
	return newVal;
}

} // namespace arch
