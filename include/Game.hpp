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
#include <ResourceManager.hpp>
#include <Speed.hpp>
#include <SpriteComponent.hpp>
#include <Velocity.hpp>
#include <chrono>
#include <memory>

#include "DropingEntity.hpp"
#include "Logs.hpp"
#include "MapDefinitions.hpp"

namespace Tetris
{
    class Game
    {
        public:
            Game();
            ~Game();

            /**
             * @brief Initialize the game
             * @return void
             */
            void init();

            /**
             * @brief Run the game loop
             * @return void
             */
            void run();

        protected:
        private:
            /**
             * @brief Initialize game entities
             * @return void
             */
            void _initEntities();

            /**
             * @brief Initialize game components
             * @return void
             */
            void _initComponents();

            /**
             * @brief Initialize game systems
             * @return void
             */
            void _initSystems();

            /**
             * @brief Initialize keybinds
             * @return void
             */
            void _initKeybinds();

            /**
             * @brief Initialize event subscriptions
             * @return void
             */
            void _initSubscriptions();

            /**
             * @brief Initialize graphics
             * @return void
             */
            void _initGraphic();

            /**
             * @brief Create the game board
             * @return void
             */
            void _createBoard(Registry& registry);

        private:
            std::shared_ptr<Graphic::Raylib> _graphic;
            GameEngine::Core _engine;
            std::unique_ptr<ResourceManager> _resource_manager;

            // Board offsets
            float _offset_x = 0.0f;
            float _offset_y = 0.0f;

            std::unique_ptr<DropingEntity> _dropingEntity;
    };
} // namespace Tetris