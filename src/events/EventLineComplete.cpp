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

        LOG_TRACE("Line {} completed with {} blocks to delete!", event.lineIndex, event.entitiesToDelete.size());
        for (Entity entity : event.entitiesToDelete) {
            registry.killEntity(entity);
            LOG_TRACE("Killed entity {}", static_cast<std::size_t>(entity));
        }
        LOG_INFO("Line {} completed!", event.lineIndex);

        // Move down blocks above the completed line
        auto& positions = registry.getComponents<Components::Position>();
        auto& colliders = registry.getComponents<Components::Collider>();

        for (auto&& [idx, pos, col] : IndexedZipper(positions, colliders)) {
            if (col.getType() != BLOCKNAME)
                continue;

            float posY  = pos.getY();
            float lineY = static_cast<float>(event.lineIndex * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y);

            // Only move blocks that are above the deleted line
            if (posY < lineY) {
                Entity entity = registry.entityFromIndex(idx);
                registry.getSpecificComponent<Components::Position>(entity).setY(posY + (HEIGHT_BLOCK * BLOCK_SCALE));
                LOG_INFO("Moved block at Y={} down by one row to Y={}", posY, posY + (HEIGHT_BLOCK * BLOCK_SCALE));
            }
        }
    });
}
