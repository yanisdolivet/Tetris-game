/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** Tetromino
*/

#pragma once

#include "MapDefinitions.hpp"

namespace Components
{
    class Tetromino
    {
        public:
            Tetromino() = default;
            Tetromino(TetrominoType type, int rotationState, int localX, int localY);
            ~Tetromino() = default;

            TetrominoType getType() const;
            int getRotationState() const;
            void setRotationState(int rotationState);

            int getLocalX() const;
            int getLocalY() const;
            void setLocalPos(int x, int y);

        private:
            TetrominoType _type;
            int _rotationState;
            int _localX;
            int _localY;
    };
}