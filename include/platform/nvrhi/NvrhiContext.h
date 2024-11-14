#pragma once

#include "Ref.h"
#include "Window.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi {

class NvrhiContext {
public:
	NvrhiContext() = default;
	virtual ~NvrhiContext() = default;

	virtual void init(const Ref<Window>& window) = 0;

public:
	virtual ::nvrhi::DeviceHandle getDevice() = 0;
};

} // namespace arch::gfx::nvrhi
