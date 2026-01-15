/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** main
*/

#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Game.hpp"
#include "Logs.hpp"

#define LOGSFOLDER "tetris_logs"

int main(void)
{
    std::srand(std::time(0));
    Logs::init(LOGSFOLDER);

    Tetris::Game tetris;

    tetris.init();
    tetris.run();

    Logs::shutdown();
    return 0;
}
