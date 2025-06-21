#pragma once

#include "input/InputTestApp.h"
#include "nvrhi_renderer/NvrhiRendererTestApp.h"
#include "physics/PhysicsTestApp.h"
#include "text_render/TextRenderTestApp.h"
// manual order, doesnt compile if sorted
#include "audio/ControlTestApp.h"
#include "audio/RewindTestApp.h"
#include "audio/SimpleSoundTestApp.h"
#include "audio/SpatialAudioTestApp.h"

// todo: some sort of container for all examples to be able to choose them at runtime
