#include <archimedes/physics/collisions/CollisionGraph.h>


namespace arch::physics {
std::unordered_map<ecs::Entity, Collision> CollisionGraph::getCollisions(ecs::Entity entity) const {
    if(_graph.contains(entity)){
        return _graph.at(entity);
    }
    return {};
}

std::vector<ecs::Entity> CollisionGraph::getCollidingEntities() const {
    std::vector<ecs::Entity> result;
    for(auto& [entity, _] : _graph) {
        result.push_back(entity);
    }
    return result;
}

void CollisionGraph::addCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision) {
    if(!_graph.contains(entity1) && !_graph.contains(entity2)){
        _graph[entity1][entity2] = collision;
    }
}

void CollisionGraph::updateCollision(ecs::Entity entity1, ecs::Entity entity2, const Collision& collision) {
    if(_graph.contains(entity1) && _graph[entity1].contains(entity2)){
        _graph[entity1][entity2] = collision;
    }
}

void CollisionGraph::removeCollision(ecs::Entity entity1, ecs::Entity entity2) {
    if(_graph.contains(entity1) && _graph[entity1].contains(entity2)){
        _graph[entity1].erase(entity2);
    }
    if(_graph[entity1].size() == 0) {
        _graph.erase(entity1);
    }
}

std::optional<Collision> CollisionGraph::getCollision(ecs::Entity entity1, ecs::Entity entity2) const {
    if(_graph.contains(entity1) && _graph.at(entity1).contains(entity2)){
        return _graph.at(entity1).at(entity2);
    }
    return std::nullopt;
}

} // namespace arch::physics