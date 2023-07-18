#ifndef ARCHIMEDES_COMMON_H
#define ARCHIMEDES_COMMON_H

#include <cstdint>
#include <cassert>
#include <bitset>
#include <spdlog/spdlog.h>

#define assert_msg(exp, msg) if (exp) { spdlog::error(msg); exit(-1); }

namespace arch::ecs {
	constexpr uint8_t max_components = 69;
	constexpr uint32_t initial_entities = 2137;

	typedef uint32_t EntityId;
	typedef uint32_t SystemId;
	typedef uint32_t ComponentId;
	typedef std::bitset<max_components> ComponentMask;

	class ComponentIdGenerator {
		static uint32_t counter;

	public:
		template <class T>
		static uint32_t get_component_id() {
			static uint32_t component_id = counter++;
			return component_id;
		}
	};

	class SystemIdGenerator {
		static uint32_t counter;

	public:
		template <class T>
		static uint32_t get_system_id() {
			static uint32_t system_id = counter++;
			return system_id;
		}
	};
}

#endif
