#include <Engine.h>
#include <Logger.h>

struct MyApp : arch::Application {};

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::EngineConfig config {
		600, 480, "Archimedes Test", glm::vec4(0, 0, 0, 0)
	};

	std::shared_ptr<MyApp> myApp = std::make_shared<MyApp>();

	arch::Engine engine { config, myApp };
	engine.start();
}