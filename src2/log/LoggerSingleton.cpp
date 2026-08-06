module;

#include <archimedes/BuildInfo.h>
#include <archimedes/Defines.h>
//
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

module arch.log;

import :LoggerSingleton;
import :LeveledLogger;

namespace arch::log::_details {

std::shared_ptr<spdlog::logger> LoggerSingleton::_logger{};

void LoggerSingleton::init(const std::string& name, bool file) {
	if (_logger) {
		log::warn("Logger was already initialized");
		return;
	}

	std::vector<spdlog::sink_ptr> sinks;

	std::string pattern = "[%T] [%l]";
	if constexpr (buildinfo::Type::current != buildinfo::Type::Release) {
		pattern += " [%@]";
	}
	pattern += ": %v";

	// init console sink
	auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	consoleSink->set_pattern("%^" + pattern + "%$");
	sinks.push_back(std::move(consoleSink));

	if (file) { // init file sink
		auto logpath = std::format("Logs/{}-{}.log", name, (long long)std::time(nullptr));
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logpath, true);
		fileSink->set_pattern(pattern);
		sinks.push_back(std::move(fileSink));
	}

	_logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
	spdlog::register_logger(_logger);

	_logger->set_level((spdlog::level::level_enum)Level::trace);
	_logger->flush_on((spdlog::level::level_enum)Level::trace);
}

} // namespace arch::log::_details
