export module arch.log;

export import :Level;
export import :LoggerSingleton;
export import :UniversalLogger;
export import :LeveledLogger;
export import :logImpl;

export namespace arch::log {

using arch::log::critical;
using arch::log::debug;
using arch::log::error;
using arch::log::info;
using arch::log::logger;
using arch::log::trace;
using arch::log::warn;

using arch::log::init;

} // namespace arch::log
