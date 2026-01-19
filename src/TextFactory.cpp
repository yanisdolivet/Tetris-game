/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** TextFactory
*/

#include "Game.hpp"

void Tetris::Game::_createMainText(Registry& registry)
{
    // Create entity
    Entity mainT                                         = registry.spawnEntity();
    const std::string text                               = "TETRIS";
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color = {25, 138, 246, 255};
    int size                                             = 42;
    std::pair<float, float> position = {static_cast<float>((WIN_WIDTH / 2) - (text.length() * size / 2)), 15.f};

    LOG_INFO("Text: {}", text);
    LOG_INFO("Position: ({}, {})", position.first, position.second);
    // Give Positions
    registry.getComponents<Components::Position>().insertAt(mainT, {position.first, position.second});

    // Give Drawable
    registry.getComponents<Components::DrawableComponent>().insertAt(mainT, {});

    // Give Text
    registry.getComponents<Components::TextComponent>().insertAt(
        mainT, {text, "TETRIS_FONT", size, 2.0f, color, this->_textId});

    this->_textId++;
}

void Tetris::Game::_createMenuText(Registry& registry)
{
    // Create entity
    Entity mainT                                         = registry.spawnEntity();
    const std::string text                               = "press Space to start";
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> color = {120, 120, 120, 128};
    int size                                             = 22;
    std::pair<float, float> position = {static_cast<float>((WIN_WIDTH / 2) - (text.length() * size / 2)),
                                        WIN_HEIGHT / 2};

    LOG_INFO("Text: {}", text);
    LOG_INFO("Position: ({}, {})", position.first, position.second);
    // Give Positions
    registry.getComponents<Components::Position>().insertAt(mainT, {position.first, position.second});

    // Give Drawable
    registry.getComponents<Components::DrawableComponent>().insertAt(mainT, {});

    // Give Text
    registry.getComponents<Components::TextComponent>().insertAt(
        mainT, {text, "TETRIS_FONT", size, 2.0f, color, this->_textId});

    this->_textId++;
}
