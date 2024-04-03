#pragma once

#include "Ref.h"
#include "TextureFilterMode.h"
#include "TextureWrapMode.h"
#include "gfx/GraphicsFormat.h"
#include "mmath.h"

namespace arch::gfx::texture {

class Texture {
protected:
	Texture() = default;
	virtual ~Texture() = default;

public:
	static Ref<Texture> create();
	static Ref<Texture> create(const void* data, u64 size);

public:
	TextureFilterMode getFilter() const { return _filterMode; }

	virtual void setFilter(TextureFilterMode filterMode) { _filterMode = filterMode; }

	TextureWrapMode getWrap() const { return _wrapMode; }

	virtual void setWrap(TextureWrapMode wrapMode) { _wrapMode = wrapMode; }

	GraphicsFormat getFormat() const { return _format; }

protected:
	TextureWrapMode _wrapMode;
	TextureFilterMode _filterMode;
	GraphicsFormat _format;
	bool _isReadable = false;
};

} // namespace arch::gfx::texture
