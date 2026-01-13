/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** Game
*/

#include "Game.hpp"

namespace Tetris
{
    Game::Game()
    {
        this->_graphic          = std::make_shared<Graphic::Raylib>();
        this->_resource_manager = std::make_unique<ResourceManager>(*_graphic, *_graphic);
        this->_dropingEntity    = std::make_unique<DropingEntity>(this->_engine.getRegistry());
    }

    Game::~Game()
    {
    }

    void Game::init()
    {
        this->_initGraphic();
        this->_initComponents();
        this->_initSystems();
        this->_initEntities();
        this->_initKeybinds();
        this->_initSubscriptions();

        LOG_INFO("Game initialized");
    }

    void Game::run()
    {
        this->_engine.start();
        this->_engine.run();
        LOG_INFO("Game Running");
    }

    void Game::_initEntities()
    {
        Registry& registry = this->_engine.getRegistry();

        this->_createBoard(registry);

        LOG_INFO("Initializing game entities");
    }

    void Game::_initComponents()
    {
        // Register all the components used in the game ex:
        this->_engine.getRegistry().registerComponent<Components::Position>();
        this->_engine.getRegistry().registerComponent<Components::Velocity>();
        this->_engine.getRegistry().registerComponent<Components::Collider>();
        this->_engine.getRegistry().registerComponent<Components::Speed>();
        this->_engine.getRegistry().registerComponent<Components::Movement>();
        this->_engine.getRegistry().registerComponent<Components::DrawableComponent>();
        this->_engine.getRegistry().registerComponent<Components::SpriteComponent>();
        this->_engine.getRegistry().registerComponent<Components::AnimationComponent>();
        this->_engine.getRegistry().registerComponent<Components::Parallax>();
        LOG_INFO("Initializing game components");
    }

    void Game::_initSystems()
    {
        // Animation System
        this->_engine.getRegistry()
            .addSystem<Components::DrawableComponent, Components::Position, Components::AnimationComponent,
                       Components::Parallax>(AnimationSystem());

        // Rendering System
        this->_engine.getRegistry()
            .addSystem<Components::Position, Components::SpriteComponent, Components::DrawableComponent>(
                RenderSystem(this->_graphic));

        // Input System
        this->_engine.getRegistry().addSystem<>(
            InputSystem(this->_graphic->getKeymap(), this->_graphic->getKeyDownFunc()));

        // Input Released System
        this->_engine.getRegistry().addSystem<>(
            InputReleasedSystem(this->_graphic->getKeyReleasedMap(), this->_graphic->getKeyReleasedFunc()));

        // Movement System
        this->_engine.getRegistry().addSystem<Components::Movement, Components::Velocity, Components::Speed>(
            MovementSystem());

        // Physics System
        this->_engine.getRegistry().addSystem<Components::Position, Components::Velocity>(PhysicsSystem());

        // Collision System
        this->_engine.getRegistry().addSystem<Components::Position, Components::Collider>(CollisionSystem());

        // Block Spawner System
        this->_engine.getRegistry().addSystem<>(BlockSpawner());

        LOG_INFO("Initializing game systems");
    }

    void Game::_initSubscriptions()
    {
        Common::initEngineSubscriptions(this->_engine);
        LOG_INFO("Initializing event subscriptions");
    }

    void Game::_initKeybinds()
    {
        const std::unordered_map<std::string, ActionBinding> actionBindings = getActionBindings();

        int key = this->_graphic->stringtoKeyCode("KEY_ESCAPE");

        auto it = actionBindings.find("ExitGame");
        if (it == actionBindings.end())
            throw std::runtime_error("Unknown action: ExitGame");
        const ActionBinding& action = it->second;

        if (action.onPress) {
            LOG_INFO("Adding action to key {}", key);
            this->_graphic->addKeyMapping(key, action.onPress);
        }

        if (action.onRelease) {
            this->_graphic->addKeyReleasedMapping(key, action.onRelease);
        }

        LOG_INFO("Initializing keybinds");
    }

    void Game::_initGraphic()
    {
        this->_graphic->initWindow(800, 600, "Tetris Game");
        const std::map<std::string, std::string>& sprites = {
            {"BLOCK", "assets/tetris-Sheet.png"},
        };
        this->_resource_manager.get()->loadTexturesFromMap(sprites);

        this->_offset_x = this->_graphic->getWindowSize().first / 2 - (BOARD_WIDTH * (WIDTH_BLOCK * 1.5f)) / 2;
        this->_offset_y = this->_graphic->getWindowSize().second - (BOARD_HEIGHT * (HEIGHT_BLOCK * 1.5f));

        LOG_INFO("Initializing graphics");
    }

} // namespace Tetris