/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** BlockId
*/

#include "BlockId.hpp"

namespace Components
{

    BlockId::BlockId() : _blockId(-1)
    {
    }

    BlockId::BlockId(int blockId) : _blockId(blockId)
    {
    }

    BlockId::~BlockId()
    {
    }

    int BlockId::getBlockId() const
    {
        return _blockId;
    }
} // namespace Components