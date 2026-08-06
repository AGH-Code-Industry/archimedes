module;

#include <archimedes/BuildInfo.h>
#include <archimedes/Defines.h>
//
#include <filesystem>
#include <format>
#include <memory>
#include <print>
#include <stacktrace>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

export module arch.log:LoggerSingleton;

import :Level;

export namespace arch::log {

void setLevel(Level level);

namespace _details {

template<class... Args>
void logImpl(Level, const unsigned int, spdlog::format_string_t<Args...>, Args&&...);

class LoggerSingleton {
public:
	/// @brief Initializes global logger
	/// @param name - logger name
	/// @param file - whether to log to a file
	static void init(const std::string& name, bool file);

private:
	template<class... Args>
	friend void ::arch::log::_details::logImpl(Level, const unsigned int, spdlog::format_string_t<Args...>, Args&&...);
	friend void ::arch::log::setLevel(Level);

	static std::shared_ptr<spdlog::logger> _logger;
};

} // namespace _details

void init(const std::string& name, bool file) {
	_details::LoggerSingleton::init(name, file);
}

void setLevel(Level level) {
	_details::LoggerSingleton::_logger->set_level((spdlog::level::level_enum)level);
}

} // namespace arch::log
