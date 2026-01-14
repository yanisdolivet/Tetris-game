/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** Tetromino
*/

#include "components/Tetromino.hpp"

namespace Components
{
    Tetromino::Tetromino(TetrominoType type, int rotationState, int localX, int localY)
        : _type(type), _rotationState(rotationState), _localX(localX), _localY(localY)
    {
    }

    TetrominoType Tetromino::getType() const
    {
        return _type;
    }

    int Tetromino::getRotationState() const
    {
        return _rotationState;
    }

    void Tetromino::setRotationState(int rotationState)
    {
        _rotationState = rotationState;
    }

    int Tetromino::getLocalX() const
    {
        return _localX;
    }

    int Tetromino::getLocalY() const
    {
        return _localY;
    }

    void Tetromino::setLocalPos(int x, int y)
    {
        _localX = x;
        _localY = y;
    }
}