#pragma once
#include "NvrhiBuffer.h"
#include "gfx/buffer/BufferManager.h"

namespace arch::gfx::nvrhi {
class NvrhiRenderer;
}

namespace arch::gfx::nvrhi::buffer {

class NvrhiBufferManager final:
	public gfx::buffer::BufferManager,
	public std::enable_shared_from_this<NvrhiBufferManager> {
public:
	NvrhiBufferManager(const WeakRef<NvrhiRenderer>& renderer);
	~NvrhiBufferManager() override = default;

protected:
	Ref<gfx::buffer::VertexBuffer> _createVertexBufferImpl(void* data, u32 size) override;
	Ref<gfx::buffer::IndexBuffer> _createIndexBufferImpl(std::span<u32> indices) override;
	Ref<gfx::buffer::Buffer> _createBufferImpl(void* data, u32 size) override;

private:
	friend class NvrhiBuffer;
	friend class NvrhiVertexBuffer;
	friend class NvrhiIndexBuffer;

	void _setBufferData(const NvrhiBuffer& buffer, const void* data, u64 size) const;

private:
	WeakRef<NvrhiRenderer> _renderer;
	::nvrhi::CommandListHandle _stageCommandBuffer;
};

} // namespace arch::gfx::nvrhi::buffer
