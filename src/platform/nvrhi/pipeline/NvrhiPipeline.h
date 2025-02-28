#pragma once

#include "Ref.h"
#include "gfx/pipeline/Pipeline.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi {
class NvrhiRenderer;
}

namespace arch::gfx::nvrhi::pipeline {

class NvrhiPipeline final: public gfx::pipeline::Pipeline {
public:
	NvrhiPipeline(const Desc& desc, const WeakRef<NvrhiRenderer>& renderer);
	~NvrhiPipeline() noexcept override = default;

	::nvrhi::GraphicsPipelineHandle getNativeHandle() const { return _pipeline; }

	::nvrhi::BindingSetHandle getNativeBindingSet() const { return _bindingSet; }

private:
	::nvrhi::GraphicsPipelineHandle _pipeline;
	::nvrhi::BindingLayoutHandle _bindingLayout;
	::nvrhi::BindingSetHandle _bindingSet;
};

} // namespace arch::gfx::nvrhi::pipeline
