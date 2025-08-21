#pragma once

#include <array>

#include <ArchMath.h>
#include <net/_UtilitiesUnix.h>
#include <net/_UtilitiesWindows.h>

namespace arch::net {

bool getMac(std::array<u8, 6>& result) noexcept;

}
