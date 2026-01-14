/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** LineDeletion
*/

#pragma once

#include "Registry.hpp"
#include "Logs.hpp"
#include "MapDefinitions.hpp"
#include "TetrisEvent.hpp"
#include "SparseArray.hpp"
#include "Position.hpp"
#include "Collider.hpp"

class LineDeletion {
    public:
        void operator()(Registry& registry, double, SparseArray<Components::Position>& positions, SparseArray<Components::Collider>& colliders) const;
};

