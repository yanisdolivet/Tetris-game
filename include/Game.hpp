/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** Game
*/

#pragma once

#include <AnimationComponent.hpp>
#include <AnimationSystem.hpp>
#include <Collider.hpp>
#include <CollisionSystem.hpp>
#include <Core.hpp>
#include <DrawableComponent.hpp>
#include <EngineSubscriptions.hpp>
#include <GameEngineEvents.hpp>
#include <InitActionInput.hpp>
#include <InputReleasedSystem.hpp>
#include <InputSystem.hpp>
#include <Movement.hpp>
#include <MovementSystem.hpp>
#include <Parallax.hpp>
#include <PhysicsSystem.hpp>
#include <Position.hpp>
#include <Raylib.hpp>
#include <RenderSystem.hpp>
#include <Speed.hpp>
#include <SpriteComponent.hpp>
#include <Velocity.hpp>
#include <memory>

#include "Logs.hpp"

namespace Tetris
{
    class Game
    {
        public:
            Game();
            ~Game();

            /*
             * @brief Initialize the game
             * @return void
             */
            void init();

            /*
             * @brief Run the game loop
             * @return void
             */
            void run();

        protected:
        private:
            /*
             * @brief Initialize game entities
             * @return void
             */
            void _initEntities();

            /*
             * @brief Initialize game components
             * @return void
             */
            void _initComponents();

            /*
             * @brief Initialize game systems
             * @return void
             */
            void _initSystems();

            /*
             * @brief Initialize keybinds
             * @return void
             */
            void _initKeybinds();

            /*
             * @brief Initialize event subscriptions
             * @return void
             */
            void _initSubscriptions();

        private:
            std::shared_ptr<Graphic::Raylib> _graphic;
            GameEngine::Core _engine;
    };
} // namespace Tetris