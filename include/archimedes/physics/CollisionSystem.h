#pragma once
#include <archimedes/ecs/Domain.h>
#include <archimedes/physics/CollisionGraph.h>

namespace arch::physics {

class CollisionSystem {
    ecs::Domain& _domain;
    CollisionGraph _savedCollisions;

    CollisionGraph _getCollidableEntities() const;
    void _checkDisappearedCollisions(const CollisionGraph& newCollisions);
    void _readCurrentCollisions(const CollisionGraph& newCollisions);

public:
    explicit CollisionSystem(ecs::Domain& domain);
    std::vector<ecs::Entity> getEnteredCollisions(ecs::Entity entity) const;
	std::vector<ecs::Entity> getExitedCollisions(ecs::Entity entity) const;
	std::vector<ecs::Entity> getLastingCollisions(ecs::Entity entity) const;
    void update();
};
}