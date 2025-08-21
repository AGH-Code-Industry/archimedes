#pragma once

#include "Pipeline.h"
#include "Ref.h"

namespace arch::gfx::pipeline {

class PipelineManager {
protected:
	PipelineManager() = default;
	virtual ~PipelineManager() = default;

public:
	virtual Ref<Pipeline> create(const Pipeline::Desc& desc) = 0;
};

} // namespace arch::gfx::pipeline
