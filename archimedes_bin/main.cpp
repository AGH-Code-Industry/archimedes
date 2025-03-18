#include "examples/NvrhiRendererTestApp.h"
#include "examples/SpatialAudioTestApp.h"
#include "examples/LegacyAudioTests.h"
#include <Engine.h>
#include <Logger.h>

int main() {
	Logger::init(LogLevel::trace);

	testSpatialAudio();

	// arch::Ref<arch::Application> myApp = arch::createRef<NvrhiRendererTestApp>();
	//
	// arch::EngineConfig config{ .windowWidth = 600,
	// 						   .windowHeight = 480,
	// 						   .windowTitle = "Archimedes Test",
	// 						   .backgroundColor = arch::Color(.03f, .03f, .03, 1.f),
	// 						   .renderingApi = arch::gfx::RenderingAPI::Nvrhi_VK };
	//
	// arch::Engine engine{ config, myApp };
	// engine.start();
}
