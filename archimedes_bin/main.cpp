#include <engine.h>
#include <Logger.h>

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Logger::trace("Hello, {}!", "World");
	arch::Logger::debug("Hello, {}!", "World");
	arch::Logger::info("Hello, {}!", "World");
	arch::Logger::warn("Hello, {}!", "World");
	arch::Logger::error("Hello, {}!", "World");
	arch::Logger::critical("Hello, {}!", "World");

	arch::Logger::log(arch::LogLevel::info, "Info log");

	arch::EngineConfig config {
		600, 480, "Archimedes Test", glm::vec4(0, 0, 0, 0)
	};

	arch::Engine engine { config };
	engine.start();
}