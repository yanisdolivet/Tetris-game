/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** DropingEntity
*/

#pragma once

#include <GameEngineEvents.hpp>
#include <cstdlib>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "BlockId.hpp"
#include "Collider.hpp"
#include "DrawableComponent.hpp"
#include "DropingEntity.hpp"
#include "Logs.hpp"
#include "MapDefinitions.hpp"
#include "Movement.hpp"
#include "Position.hpp"
#include "Registry.hpp"
#include "Speed.hpp"
#include "SpriteComponent.hpp"
#include "Velocity.hpp"
#include "TetrisEvent.hpp"

namespace Tetris
{
    class DropingEntity
    {
        public:
            DropingEntity(Registry& registry);
            ~DropingEntity();

            void generateBlock(BlockColor color, int row, std::string blockType);
            void spawnBlock();

            // Test functions for line deletion feature
            void createTestLine(int lineY);
            void createAlmostCompleteLine(int lineY, int missingCol);

        protected:
        private:
            std::vector<Entity> generateL(int row, SparseArray<Components::Position>& positions,
                                          SparseArray<Components::Movement>& movements,
                                          SparseArray<Components::BlockId>& blockids) const;
            std::vector<Entity> generateO(int row, SparseArray<Components::Position>& positions,
                                          SparseArray<Components::Movement>& movements,
                                          SparseArray<Components::BlockId>& blockids) const;
            std::vector<Entity> generateI(int row, SparseArray<Components::Position>& positions,
                                          SparseArray<Components::Movement>& movements,
                                          SparseArray<Components::BlockId>& blockids) const;
            std::vector<Entity> generateS(int row, SparseArray<Components::Position>& positions,
                                          SparseArray<Components::Movement>& movements,
                                          SparseArray<Components::BlockId>& blockids) const;
            std::vector<Entity> generateZ(int row, SparseArray<Components::Position>& positions,
                                          SparseArray<Components::Movement>& movements,
                                          SparseArray<Components::BlockId>& blockids) const;
            std::vector<Entity> generateJ(int row, SparseArray<Components::Position>& positions,
                                          SparseArray<Components::Movement>& movements,
                                          SparseArray<Components::BlockId>& blockids) const;
            std::vector<Entity> generateT(int row, SparseArray<Components::Position>& positions,
                                          SparseArray<Components::Movement>& movements,
                                          SparseArray<Components::BlockId>& blockids) const;

            Registry& _registry;

            int _currentBlockId;
    };

} // namespace Tetris
