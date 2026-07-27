#include <archimedes/BuildInfo.h>
#include <archimedes/Logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace arch::log {

namespace _details {

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

	_logger->set_level((spdlog::level::level_enum)Level::info);
	_logger->flush_on((spdlog::level::level_enum)Level::debug);
}

} // namespace _details

void setLevel(Level level) {
	_details::LoggerSingleton::_logger->set_level((spdlog::level::level_enum)level);
}

_details::UniversalLogger logger{};

_details::LeveledLogger<Level::trace> trace{};
_details::LeveledLogger<Level::debug> debug{};
_details::LeveledLogger<Level::info> info{};
_details::LeveledLogger<Level::warn> warn{};
_details::LeveledLogger<Level::error> error{};
_details::LeveledLogger<Level::critical> critical{};

} // namespace arch::log
