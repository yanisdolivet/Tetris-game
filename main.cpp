/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** main
*/

#include <iostream>

#include "Game.hpp"
#include "Logs.hpp"
#include <cstdlib>
#include <ctime>

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
