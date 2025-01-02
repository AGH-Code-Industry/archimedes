#include "examples/NvrhiRendererTestApp.h"
#include <Engine.h>
#include <Logger.h>
#include <audio/AudioManager.h>
#include <audio/SoundDevice.h>

const std::string sounds = "/home/anon/dev/archimedes/archimedes_bin/sounds/";

namespace audio = arch::audio;
namespace ecs = arch::ecs;

int main() {
	arch::Logger::init(arch::LogLevel::trace);

	arch::Ref<arch::Application> myApp = arch::createRef<NvrhiRendererTestApp>();

	arch::EngineConfig config{ .windowWidth = 600,
							   .windowHeight = 480,
							   .windowTitle = "Archimedes Test",
							   .backgroundColor = arch::Color(.03f, .03f, .03, 1.f),
							   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };

	// testSimpleSound(domain);
	// testControl(domain);
	testSpatialAudio(domain);

	// arch::Ref<MyApp> myApp = arch::createRef<MyApp>();
	//
	// arch::Engine engine { config, myApp };
	// engine.start();
}
