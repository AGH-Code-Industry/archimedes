#include <engine.h>
#include <spdlog/spdlog.h>

int main() {
	spdlog::set_level(spdlog::level::debug);
	arch::EngineConfig config {
		600, 480, "Archimedes Test", glm::vec4(0, 0, 0, 0)
	};

	arch::Engine engine { config };
	engine.start();
}
