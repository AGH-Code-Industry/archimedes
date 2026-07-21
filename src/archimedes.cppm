module;

#ifndef SPDLOG_USE_STD_FORMAT

#define SPDLOG_USE_STD_FORMAT 1

#endif

#if defined(_MSC_VER) && defined(_WIN32)

#define ARCHIMEDES_WINDOWS 1
#define ARCHIMEDES_LINUX 0
#define ARCHIMEDES_UNIX 0

#elif defined(__GNUC__) && defined(__linux__)

#define ARCHIMEDES_WINDOWS 0
#define ARCHIMEDES_LINUX 1
#define ARCHIMEDES_UNIX 1

#endif

#if defined(NDEBUG)

#define ARCHIMEDES_DEBUG 0
#define ARCHIMEDES_RELEASE 1

#else

#define ARCHIMEDES_DEBUG 1
#define ARCHIMEDES_RELEASE 0

#endif

// asset
#include <archimedes/asset/mesh/Mesh.h>

// audio
#include <archimedes/audio/Calls.hpp>

#include <archimedes/audio/AudioException.h>
#include <archimedes/audio/AudioManager.h>
#include <archimedes/audio/AudioSourceComponent.h>
#include <archimedes/audio/Clip.h>
#include <archimedes/audio/Listener.h>
#include <archimedes/audio/ListenerComponent.h>
#include <archimedes/audio/SoundBank.h>
#include <archimedes/audio/SoundDevice.h>
#include <archimedes/audio/SourcePlayer.h>

// ecs
#include <archimedes/Ecs.h>

// exceptions
#include <archimedes/exceptions/ConfigException.h>
#include <archimedes/exceptions/GLFWException.h>
#include <archimedes/exceptions/InitException.h>
#include <archimedes/exceptions/InvalidEnumException.h>

// font
#include <archimedes/Font.h>

// gfx
#include <archimedes/Gfx.h>

// hier
#include <archimedes/Hier.h>

// input
#include <archimedes/Input.h>

// math
#include <archimedes/ArchMath.h>

// meta
#include <archimedes/Meta.h>

// net
#include <archimedes/Net.h>

// physics
#include <archimedes/physics/System.h>
#include <archimedes/physics/components/BBox.h>
#include <archimedes/physics/components/Colliding.h>
#include <archimedes/physics/components/Mass.h>
#include <archimedes/physics/components/MassCenter.h>
#include <archimedes/physics/components/Moveable.h>
#include <archimedes/physics/components/Velocity.h>

// resource - empty

// scene
#include <archimedes/Scene.h>

// text
#include <archimedes/Text.h>

// tUtils
#include <archimedes/TUtils.h>

// utils
#include <archimedes/utils/BoundedRange.h>
#include <archimedes/utils/MoveFlag.h>
#include <archimedes/utils/OptRef.h>
#include <archimedes/utils/PairHasher.h>
#include <archimedes/utils/ReadonlyCounter.h>
#include <archimedes/utils/Res.h>
#include <archimedes/utils/StringViewComparator.h>
#include <archimedes/utils/StringViewHasher.h>
#include <archimedes/utils/UUID.h>

// standalones
#include <archimedes/Application.h>
#include <archimedes/Camera.h>
#include <archimedes/Engine.h>
#include <archimedes/Exception.h>
#include <archimedes/Logger.h>
#include <archimedes/Monitor.h>
#include <archimedes/Ref.h>
#include <archimedes/Window.h>

export module archimedes;

export namespace arch::audio {

// @Nitka TODO

}

export namespace arch::physics {

// @Nitka TODO

}

export namespace arch::asset::mesh {

using arch::asset::mesh::Mesh;

}

export namespace arch::ecs {

using arch::ecs::ComponentPool;
using arch::ecs::ComponentSpecs;
using arch::ecs::Domain;
using arch::ecs::Entity;
using arch::ecs::EntityPool;
using arch::ecs::EntitySpecs;
using arch::ecs::exclude;
using arch::ecs::ExcludeT;
using arch::ecs::nullEntity;
using arch::ecs::nullID;
using arch::ecs::nullVersion;
using arch::ecs::operator*;
using arch::ecs::operator~;
using arch::ecs::View;
using arch::ecs::ViewIterator;

using arch::ecs::_details::ComponentPoolIterator;
using arch::ecs::_details::ComponentTraits;
using arch::ecs::_details::EntityTraits;
using arch::ecs::_details::FlagComponent;
using arch::ecs::_details::FlagComponentPred;
using arch::ecs::_details::IsEntity;

} // namespace arch::ecs

export namespace arch::utils {

using arch::utils::UUID;

}
