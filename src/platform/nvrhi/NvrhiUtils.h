#pragma once
#include "gfx/GraphicsFormat.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi {

class NvrhiUtils {
public:
	static ::nvrhi::Format getFormat(GraphicsFormat format);
};

} // namespace arch::gfx::nvrhi
