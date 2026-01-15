/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** BlockId
*/

#pragma once

namespace Components
{

    class BlockId
    {
        public:
            BlockId();
            BlockId(int blockId);
            ~BlockId();

            int getBlockId() const;

        protected:
        private:
            int _blockId = -1;
    };
} // namespace Components