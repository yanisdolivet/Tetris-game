/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** Game
*/

#pragma once

#include <AnimationComponent.hpp>
#include <AnimationSystem.hpp>
#include <AudioSystem.hpp>
#include <Collider.hpp>
#include <CollisionSystem.hpp>
#include <Core.hpp>
#include <DrawableComponent.hpp>
#include <EngineSubscriptions.hpp>
#include <GameEngineEvents.hpp>
#include <InitActionInput.hpp>
#include <InputPressedSystem.hpp>
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
#include <SceneManager.hpp>
#include <Speed.hpp>
#include <SpriteComponent.hpp>
#include <TextComponent.hpp>
#include <Velocity.hpp>
#include <chrono>
#include <memory>
#include <tuple>

#include "BlockId.hpp"
#include "BlockSpawner.hpp"
#include "DropingEntity.hpp"
#include "LineDeletion.hpp"
#include "Logs.hpp"
#include "MapDefinitions.hpp"
#include "TetrisEvent.hpp"
#include "Tetromino.hpp"
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

            /**
             * @brief Create main text
             * @param registry
             * @return void
             */
            void _createMainText(Registry& registry);

            /**
             * @brief Register collision event
             * @param engine
             * @return void
             */
            void _registerEventCollision(GameEngine::Core& engine);

            /**
             * @brief Register spawn block event
             * @param engine
             * @return void
             */
            void _registerEventSpawnBlock(GameEngine::Core& engine);

            /**
             * @brief Register line complete event
             * @param engine
             * @return void
             */
            void _registerEventGameOver(GameEngine::Core& engine);

            /**
             * @brief Register rotate counter-clockwise event
             * @param engine
             * @return void
             */
            void _registerEventRotateCntClockwise(GameEngine::Core& engine);

            /**
             * @brief Register rotate clockwise event
             * @param engine
             * @return void
             */
            void _registerEventRotateClockwise(GameEngine::Core& engine);

            /**
             * @brief Register move event
             * @param engine
             * @return void
             */
            void _registerEventMove(GameEngine::Core& engine);

            /**
             * @brief Register line complete event
             * @param engine
             * @return void
             */
            void _registerEventLineComplete(GameEngine::Core& engine);

            /**
             * @brief Initialize scenes
             * @return void
             */
            void _initScenes();

            /**
             * @brief Kill all entities
             * @return void
             */
            void _killAllEntities();

            /**
             * @brief Create menu text
             * @param registry
             * @return void
             */
            void _createMenuText(Registry& registry);

        private:
            std::shared_ptr<Graphic::Raylib> _graphic;
            GameEngine::Core _engine;
            std::unique_ptr<ResourceManager> _resource_manager;
            SceneManager _scene_manager;

            // Board offsets
            float _offset_x = 0.0f;
            float _offset_y = 0.0f;

            std::unique_ptr<DropingEntity> _dropingEntity;
            int _textId = 0;
            std::string _current_scene;
    };
} // namespace Tetris