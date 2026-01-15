/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** LineDeletion
*/

#pragma once

#include "Collider.hpp"
#include "Logs.hpp"
#include "MapDefinitions.hpp"
#include "Movement.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "SparseArray.hpp"
#include "TetrisEvent.hpp"

class LineDeletion
{
    public:
        void operator()(Registry& registry, double, SparseArray<Components::Position>& positions,
                        SparseArray<Components::Collider>& colliders,
                        SparseArray<Components::Movement>& movements) const;
};
