#pragma once

#include "nvrhi/nvrhi.h"
#include <archimedes/Logger.h>

namespace arch::gfx::nvrhi {

struct MessageCallback: ::nvrhi::IMessageCallback {
	void message(::nvrhi::MessageSeverity severity, const char* message) override {
		auto level = log::Level::debug;

		switch (severity) {
			case ::nvrhi::MessageSeverity::Fatal:	level = log::Level::crit; break;
			case ::nvrhi::MessageSeverity::Error:	level = log::Level::error; break;
			case ::nvrhi::MessageSeverity::Warning: level = log::Level::warn; break;
			case ::nvrhi::MessageSeverity::Info:	level = log::Level::info; break;
		}

		log::logger(level, "[Nvrhi]: {}", message);
	}

	static MessageCallback* GetInstance() {
		static MessageCallback instance;
		return &instance;
	}
};

} // namespace arch::gfx::nvrhi
