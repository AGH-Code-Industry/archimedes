#pragma once

#include "Logger.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi {

struct MessageCallback: ::nvrhi::IMessageCallback {
	void message(::nvrhi::MessageSeverity severity, const char* message) override {
		LogLevel level = LogLevel::debug;

		switch (severity) {
			case ::nvrhi::MessageSeverity::Fatal:	level = LogLevel::critical; break;
			case ::nvrhi::MessageSeverity::Error:	level = LogLevel::error; break;
			case ::nvrhi::MessageSeverity::Warning: level = LogLevel::warn; break;
			case ::nvrhi::MessageSeverity::Info:	level = LogLevel::info; break;
		}

		Logger::log(level, "[Nvrhi]: {}", message);
	}

	static MessageCallback* GetInstance() {
		static MessageCallback instance;
		return &instance;
	}
};

} // namespace arch::gfx::nvrhi
