/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** EventGameOver
*/

#include "Game.hpp"

void Tetris::Game::_registerEventGameOver(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventGameOver>([this](const EventGameOver&) {
        Registry& registry = this->_engine.getRegistry();

        auto& positions = registry.getComponents<Components::Position>();

        std::vector<Entity> entitiesToKill;
        for (auto [idx, pos] : IndexedZipper(positions)) {
            if (positions[idx].has_value()) {
                entitiesToKill.push_back(static_cast<Entity>(idx));
            }
        }

        // Kill all entities
        for (Entity entity : entitiesToKill) {
            registry.killEntity(entity);
            LOG_TRACE("Killed entity {}", static_cast<std::size_t>(entity));
        }

        LOG_INFO("Killed {} entities", entitiesToKill.size());
        LOG_FATAL("Game Over, Killing all entities");
    });
}