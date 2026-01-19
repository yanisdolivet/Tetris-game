/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** BlockSpawner
*/

#pragma once

#include <chrono>

#include "Logs.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "SparseArray.hpp"
#include "SpriteComponent.hpp"
#include "TetrisEvent.hpp"

class BlockSpawner
{
    public:
        BlockSpawner(float offset_y) : _offsetY(offset_y) {};
        void operator()(Registry& registry, double deltaTime, SparseArray<Components::Position> positions,
                        SparseArray<Components::SpriteComponent> sprites) const;

    private:
        float _offsetY;
};
