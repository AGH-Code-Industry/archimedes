#pragma once

#include "gfx/pipeline/PipelineManager.h"

namespace arch::gfx::nvrhi {
class NvrhiRenderer;
}

namespace arch::gfx::nvrhi::pipeline {

class NvrhiPipelineManager final:
	public gfx::pipeline::PipelineManager,
	public std::enable_shared_from_this<NvrhiPipelineManager> {
public:
	explicit NvrhiPipelineManager(const WeakRef<NvrhiRenderer>& renderer): _renderer(renderer) {}

	~NvrhiPipelineManager() override = default;

public:
	Ref<gfx::pipeline::Pipeline> create(const gfx::pipeline::Pipeline::Desc& desc) override;

private:
	WeakRef<NvrhiRenderer> _renderer;
};

} // namespace arch::gfx::nvrhi::pipeline
