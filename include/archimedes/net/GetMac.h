#pragma once

#include <array>

#include <archimedes/ArchMath.h>
#include <archimedes/net/_UtilitiesUnix.h>
#include <archimedes/net/_UtilitiesWindows.h>

namespace arch::net {

bool getMac(std::array<u8, 6>& result) noexcept;

}
