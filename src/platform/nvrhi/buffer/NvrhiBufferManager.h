#pragma once
#include "gfx/buffer/BufferManager.h"

namespace arch::gfx::nvrhi::buffer {

class NvrhiBufferManager final: public gfx::buffer::BufferManager {
public:
	NvrhiBufferManager() = default;
	~NvrhiBufferManager() override = default;

protected:
	Ref<gfx::buffer::VertexBuffer> _createVertexBufferImpl(void* data, u32 size) override;
	Ref<gfx::buffer::IndexBuffer> _createIndexBufferImpl(std::span<u32> indices) override;
	Ref<gfx::buffer::Buffer> _createBufferImpl(void* data, u32 size) override;
};

} // namespace arch::gfx::nvrhi::buffer
