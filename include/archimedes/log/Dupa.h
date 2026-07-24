#pragma once

#include <archimedes/Mmath.h>
#include <archimedes/log/Logger.h>

namespace arch {

extern u32 dupa(log::Level level = log::crit) noexcept;
extern u32 dupa(const u32 newVal, log::Level = log::crit) noexcept;

} // namespace arch
