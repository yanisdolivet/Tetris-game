/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** EventCollision
*/

#include "Game.hpp"

void Tetris::Game::_registerEventCollision(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventCollision>([&engine](const EventCollision& event) {
        auto& registry      = engine.getRegistry();
        auto& blockIds      = registry.getComponents<Components::BlockId>();
        std::string entityA = event.entityAName;
        std::string entityB = event.entityBName;

        // Avoid collision between walls
        if (entityA == "WALL" && entityB == "WALL") {
            return;
        }

        if (entityA == "BLOCK") {
            Entity entity = registry.entityFromIndex(event.entityA);
            registry.getSpecificComponent<Components::Movement>(entity).setVertical(0);

            // Get the block ID of the colliding block
            if (!registry.entity_has_component<Components::BlockId>(entity))
                return;
            int entity_block = registry.getSpecificComponent<Components::BlockId>(entity).getBlockId();

            // LOG_INFO("Colliding Block ID: {}", entity_block);

            // Get all BlockId components
            for (auto&& [idxA, blid] : IndexedZipper(blockIds)) {
                if (static_cast<Entity>(idxA) == event.entityA)
                    continue;

                // Check if the other block has the same BlockId
                int other_block = blid.getBlockId();

                if (entity_block == other_block) {
                    // Set vertical movement of the other block to 0
                    Entity other_entity = registry.entityFromIndex(idxA);
                    if (registry.entity_has_component<Components::Movement>(other_entity)) {
                        registry.getSpecificComponent<Components::Movement>(other_entity).setVertical(0);
                        // LOG_INFO("Block entity {} collided and vertical movement set to 0", event.entityA);
                    }
                }
            }
        }
        if (entityB == "BLOCK") {
            Entity entity = registry.entityFromIndex(event.entityB);
            registry.getSpecificComponent<Components::Movement>(entity).setVertical(0);

            // Get the block ID of the colliding block
            if (!registry.entity_has_component<Components::BlockId>(entity))
                return;
            int entity_block = registry.getSpecificComponent<Components::BlockId>(entity).getBlockId();

            // LOG_INFO("Colliding Block ID: {}", entity_block);

            // Get all BlockId components
            for (auto&& [idxA, blid] : IndexedZipper(blockIds)) {
                if (static_cast<Entity>(idxA) == event.entityB)
                    continue;

                // Check if the other block has the same BlockId
                int other_block = blid.getBlockId();
                if (entity_block == other_block) {
                    // Set vertical movement of the other block to 0
                    Entity other_entity = registry.entityFromIndex(idxA);
                    if (registry.entity_has_component<Components::Movement>(other_entity)) {
                        registry.getSpecificComponent<Components::Movement>(other_entity).setVertical(0);
                        // LOG_INFO("Block entity {} collided and vertical movement set to 0", event.entityB);
                    }
                }
            }
        }
    });
}