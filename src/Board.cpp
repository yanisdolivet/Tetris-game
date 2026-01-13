/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** Board
*/

#include "Game.hpp"

namespace Tetris
{

    void createFirstAndLastRow(Registry& registry, float offset_x, float offset_y)
    {
        int row = 0;
        for (int i = 0; i < 2; ++i) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                // Create wall enttity
                auto entity                             = registry.spawnEntity();
                GameEngine::Math::Rectangle source_rect = {160.0f, 0.0f, WIDTH_BLOCK, HEIGHT_BLOCK};
                GameEngine::Math::Vector2 scale         = {1.5f, 1.5f};
                int layer                               = 0;

                registry.getComponents<Components::DrawableComponent>().insertAt(
                    entity, Components::DrawableComponent{source_rect, true, scale, layer});

                registry.getComponents<Components::SpriteComponent>().insertAt(
                    entity, Components::SpriteComponent{"BLOCK", layer});

                registry.getComponents<Components::Position>().insertAt(
                    entity, Components::Position{static_cast<float>(x * (WIDTH_BLOCK * scale.x) + offset_x),
                                                 static_cast<float>(row * (HEIGHT_BLOCK * scale.y) + offset_y)});
            }
            row = BOARD_HEIGHT - 1;
        }
    }

    void Game::_createBoard(Registry& registry)
    {
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            for (int x = 0; x < BOARD_WIDTH; x += (BOARD_WIDTH - 1)) {
                // Create wall enttity
                auto entity                             = registry.spawnEntity();
                GameEngine::Math::Rectangle source_rect = {160.0f, 0.0f, WIDTH_BLOCK, HEIGHT_BLOCK};
                GameEngine::Math::Vector2 scale         = {1.5f, 1.5f};
                int layer                               = 0;

                registry.getComponents<Components::DrawableComponent>().insertAt(
                    entity, Components::DrawableComponent{source_rect, true, scale, layer});

                registry.getComponents<Components::SpriteComponent>().insertAt(
                    entity, Components::SpriteComponent{"BLOCK", layer});

                registry.getComponents<Components::Position>().insertAt(
                    entity, Components::Position{static_cast<float>(x * (WIDTH_BLOCK * scale.x) + this->_offset_x),
                                                 static_cast<float>(y * (HEIGHT_BLOCK * scale.y) + this->_offset_y)});
            }
        }
        createFirstAndLastRow(registry, this->_offset_x, this->_offset_y);
    }
} // namespace Tetris