#include "ecs/entity.h"

using namespace arch::ecs;

Entity::Entity() : id(0), mask(ComponentMask()) {}
Entity::Entity(EntityId id, ComponentMask mask) : id(id), mask(mask) {}