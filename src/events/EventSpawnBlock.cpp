/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** EventSpawnBlock
*/

#include "Game.hpp"

void Tetris::Game::_registerEventSpawnBlock(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventSpawnBlock>([this](const EventSpawnBlock&) {
        this->_dropingEntity->spawnBlock();
        LOG_INFO("Spawned new block");
    });
}

