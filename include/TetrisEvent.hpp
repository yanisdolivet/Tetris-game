/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** TetrisEvent
*/

#pragma once

#include "Entity.hpp"

struct EventSpawnBlock
{
};

struct EventGameOver
{
};

struct EventLineComplete
{
    int lineIndex;
    std::vector<Entity> entitiesToDelete;
};

struct EventRotateCntClockwise
{
    bool pressed;
};

struct EventRotateClockwise
{
    bool pressed;
};