/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** EventLineComplete
*/

#include "Game.hpp"

void Tetris::Game::_registerEventLineComplete(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventLineComplete>([this](const EventLineComplete& event) {
        Registry& registry = this->_engine.getRegistry();

        for (Entity entity : event.entitiesToDelete) {
            registry.killEntity(entity);
            LOG_TRACE("Killed entity {}", static_cast<std::size_t>(entity));
        }
        LOG_INFO("Line {} completed!", event.lineIndex);
    });
}
