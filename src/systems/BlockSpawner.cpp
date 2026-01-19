/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** BlockSpawner
*/

#include "BlockSpawner.hpp"

void BlockSpawner::operator()(Registry& registry, double deltaTime, SparseArray<Components::Position> positions,
                              SparseArray<Components::SpriteComponent> sprites) const
{
    static double _lastSpawnTime = 0.0;
    static double _spawnInterval = 5.0; // Spawn every 5 seconds
    _lastSpawnTime += deltaTime;

    if (_lastSpawnTime >= _spawnInterval) {
        registry.publish(EventSpawnBlock{});
        _lastSpawnTime = 0.0;
    }

    for (auto&& [idx, pos, sprite] : IndexedZipper(positions, sprites)) {
        if (sprite.getResourceId() == "BLOCK" && pos.getY() >= _offsetY) {
            Entity ent = registry.entityFromIndex(idx);
            registry.publish(EventDisplayBlock{ent});
        }
    }
}
