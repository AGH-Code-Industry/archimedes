#pragma once
#include "nvrhi/nvrhi.h"
#include <archimedes/gfx/GraphicsFormat.h>

namespace arch::gfx::nvrhi {

class NvrhiUtils {
public:
	static ::nvrhi::Format getFormat(GraphicsFormat format);
};

} // namespace arch::gfx::nvrhi
