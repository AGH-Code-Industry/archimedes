#include <archimedes/physics/CollisionGraph.h>


namespace arch::physics {
std::unordered_map<ecs::Entity, CollisionState> CollisionGraph::getCollisions(ecs::Entity entity) const {
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

void CollisionGraph::addCollision(ecs::Entity entity1, ecs::Entity entity2, CollisionState state) {
    _graph[entity1][entity2] = state;
    _graph[entity2][entity1] = state;
}

void CollisionGraph::changeCollisionState(ecs::Entity entity1, ecs::Entity entity2, CollisionState toState) {
    if(_graph.contains(entity1) && _graph[entity1].contains(entity2)){
        _graph[entity1][entity2] = toState;
        _graph[entity2][entity1] = toState;
    }
}

void CollisionGraph::removeCollision(ecs::Entity entity1, ecs::Entity entity2) {
    if(_graph.contains(entity1)){
        _graph[entity1].erase(entity2);
    }
    if(_graph.contains(entity2)){
        _graph[entity2].erase(entity1);
    }
}

CollisionState CollisionGraph::getCollisionState(ecs::Entity entity1, ecs::Entity entity2) const {
    if(_graph.contains(entity1) && _graph.at(entity1).contains(entity2)){
        return _graph.at(entity1).at(entity2);
    }
    return CollisionState::NotExisting;
}

} // namespace arch::physics