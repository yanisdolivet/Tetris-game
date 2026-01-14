/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** EventInput
*/

#include "Game.hpp"

void Tetris::Game::_registerEventRotateCntClockwise(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventRotateCntClockwise>([&](const EventRotateCntClockwise&) {
        LOG_INFO("Rotate Counter-Clockwise event received");
    });
}