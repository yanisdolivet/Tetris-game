/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** BlockSpawner
*/

#pragma once

#include "Registry.hpp"
#include "Logs.hpp"
#include <chrono>
#include "TetrisEvent.hpp"

class BlockSpawner {
    public:
        void operator()(Registry& registry, double deltaTime) const;

    private:
};
