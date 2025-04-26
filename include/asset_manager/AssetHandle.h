#pragma once

#include <atomic>
#include <memory>
#include <string>

#include "AssetException.h"

namespace arch::assetManager {

enum class AssetState : uint8_t {
	Loading,
	Ready,
	Error
};

struct AssetEntryBase {
	std::atomic<AssetState> state{ AssetState::Loading };
	std::string err;
	virtual ~AssetEntryBase() = default;
};

template<class T>
struct AssetEntry final: AssetEntryBase {
	std::shared_ptr<T> ptr;
	bool loadQueued{ false };
};

template<class T>
class AssetHandle {
public:
	bool IsReady() const noexcept { return _entry->state == AssetState::Ready; }

	bool HasError() const noexcept { return _entry->state == AssetState::Error; }

	const T* Get() const noexcept { return IsReady() ? static_cast<AssetEntry<T>*>(_entry.get())->ptr.get() : nullptr; }

	explicit operator bool() const noexcept { return IsReady(); }

	const std::string& ErrorMsg() const { return _entry->err; }

private:
	std::shared_ptr<AssetEntryBase> _entry;

	explicit AssetHandle(std::shared_ptr<AssetEntryBase> e): _entry(std::move(e)) {}
	friend class AssetManager;
};

} // namespace arch::assetManager
