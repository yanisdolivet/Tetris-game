/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** BlockSpawner
*/

#pragma once

#include <chrono>

#include "Logs.hpp"
#include "Registry.hpp"
#include "TetrisEvent.hpp"

class BlockSpawner
{
    public:
        void operator()(Registry& registry, double deltaTime) const;

    private:
};
