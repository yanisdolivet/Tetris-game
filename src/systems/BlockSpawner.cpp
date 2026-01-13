/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** BlockSpawner
*/

#include "BlockSpawner.hpp"

#include "DropingEntity.hpp"

void BlockSpawner::operator()(Registry& registry, double deltaTime) const
{
    static double _lastSpawnTime = 0.0;
    static double _spawnInterval = 5.0; // Spawn every 5 seconds
    _lastSpawnTime += deltaTime;

    if (_lastSpawnTime >= _spawnInterval) {
        Tetris::DropingEntity dropingEntity(registry);
        dropingEntity.spawnBlock();

        // Reset the timer
        _lastSpawnTime = 0.0;
    }
}
