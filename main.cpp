/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** main
*/

#include <iostream>

#include "Game.hpp"
#include "Logs.hpp"

#define LOGSFOLDER "tetris_logs"

int main(void)
{
    Logs::init(LOGSFOLDER);

    Tetris::Game tetris;

    tetris.init();
    tetris.run();

    Logs::shutdown();
    return 0;
}
