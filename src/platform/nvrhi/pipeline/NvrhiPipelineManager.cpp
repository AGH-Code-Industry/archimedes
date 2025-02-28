#include "NvrhiPipelineManager.h"

#include "../NvrhiRenderer.h"
#include "NvrhiPipeline.h"

namespace arch::gfx::nvrhi::pipeline {
class NvrhiPipeline;

Ref<gfx::pipeline::Pipeline> NvrhiPipelineManager::create(const gfx::pipeline::Pipeline::Desc& desc) {
	return createRef<NvrhiPipeline>(desc, _renderer);
}

} // namespace arch::gfx::nvrhi::pipeline
