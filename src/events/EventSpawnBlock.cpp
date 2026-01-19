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

    engine.getRegistry().subscribe<EventDisplayBlock>([this, &engine](const EventDisplayBlock& event) {
        Entity entity = event.entity;

        if (engine.getRegistry().entity_has_component<Components::DrawableComponent>(entity)) {
            engine.getRegistry().getSpecificComponent<Components::DrawableComponent>(entity).changeVisibility(true);
            LOG_INFO("Displayed block entity {}", static_cast<std::size_t>(entity));
        }
    });
}
