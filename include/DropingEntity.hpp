/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** DropingEntity
*/

#pragma once

#include <cstdlib>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

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

namespace Tetris
{
    class DropingEntity
    {
        public:
            DropingEntity(Registry& registry);
            ~DropingEntity();

            void generateBlock(BlockColor color, int row, std::string blockType);
            void spawnBlock();

        protected:
        private:
            std::vector<Entity> generateL(int row) const;
            std::vector<Entity> generateO(int row) const;
            std::vector<Entity> generateI(int row) const;
            std::vector<Entity> generateS(int row) const;
            std::vector<Entity> generateZ(int row) const;
            std::vector<Entity> generateJ(int row) const;
            std::vector<Entity> generateT(int row) const;

            Registry& _registry;
    };

} // namespace Tetris
