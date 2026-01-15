/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** EventCollision
*/

#include "Game.hpp"

namespace
{
    /**
     * @brief Stop the vertical movement of a specific entity
     *
     * @param registry The game registry
     * @param entity The entity to stop
     */
    void stopEntityMovement(Registry& registry, Entity entity)
    {
        if (registry.entity_has_component<Components::Movement>(entity)) {
            registry.getSpecificComponent<Components::Movement>(entity).setVertical(0);
        }
    }

    /**
     * @brief Stop all blocks with the same BlockId
     *
     * @param registry The game registry
     * @param blockId The BlockId to match
     * @param excludeEntity Entity to exclude from the search (the one that triggered collision)
     */
    void stopBlockGroup(Registry& registry, int blockId, Entity excludeEntity)
    {
        auto& blockIds = registry.getComponents<Components::BlockId>();

        for (auto&& [idx, blid] : IndexedZipper(blockIds)) {
            if (static_cast<Entity>(idx) == excludeEntity) {
                continue;
            }

            if (blid.getBlockId() == blockId) {
                Entity entity = registry.entityFromIndex(idx);
                stopEntityMovement(registry, entity);
            }
        }
    }

    /**
     * @brief Handle collision for a block entity
     *
     * @param registry The game registry
     * @param entityIndex The index of the entity that collided
     */
    void handleBlockCollision(Registry& registry, size_t entityIndex)
    {
        Entity entity = registry.entityFromIndex(entityIndex);

        // Stop the entity's vertical movement
        stopEntityMovement(registry, entity);

        // If entity doesn't have BlockId, no group to stop
        if (!registry.entity_has_component<Components::BlockId>(entity)) {
            return;
        }

        // Get the block ID and stop all blocks in the same group
        int blockId = registry.getSpecificComponent<Components::BlockId>(entity).getBlockId();
        stopBlockGroup(registry, blockId, entity);
    }

    /**
     * @brief Check if collision should be ignored
     *
     * @param entityAName Name of first entity
     * @param entityBName Name of second entity
     * @return true if collision should be ignored
     */
    bool shouldIgnoreCollision(const std::string& entityAName, const std::string& entityBName)
    {
        // Avoid collision between walls
        return (entityAName == "WALL" && entityBName == "WALL");
    }
} // anonymous namespace

void Tetris::Game::_registerEventCollision(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventCollision>([&engine](const EventCollision& event) {
        auto& registry      = engine.getRegistry();
        std::string entityA = event.entityAName;
        std::string entityB = event.entityBName;

        // Skip collisions that should be ignored
        if (shouldIgnoreCollision(entityA, entityB)) {
            return;
        }

        // Handle block collisions
        if (entityA == "BLOCK") {
            handleBlockCollision(registry, event.entityA);
        }
        if (entityB == "BLOCK") {
            handleBlockCollision(registry, event.entityB);
        }
    });
}