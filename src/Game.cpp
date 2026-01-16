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
    }

    void Game::_initEntities()
    {
        Registry& registry = this->_engine.getRegistry();

        this->_createBoard(registry);
        this->_createMainText(registry);

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
        this->_engine.getRegistry().registerComponent<Components::BlockId>();
        this->_engine.getRegistry().registerComponent<Components::Tetromino>();
        this->_engine.getRegistry().registerComponent<Components::TextComponent>();

        LOG_INFO("Initializing game components");
    }

    void Game::_initSystems()
    {
        // Animation System
        this->_engine.getRegistry()
            .addSystem<Components::DrawableComponent, Components::Position, Components::AnimationComponent,
                       Components::Parallax>(AnimationSystem());

        // Rendering System
        this->_engine.getRegistry().addSystem<Components::Position, Components::DrawableComponent>(
            RenderSystem(this->_graphic));

        // Input System
        this->_engine.getRegistry().addSystem<>(
            InputPressedSystem(this->_graphic->getKeymap(), this->_graphic->getKeyDownFunc()));

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

        // Line Deletion System
        this->_engine.getRegistry().addSystem<Components::Position, Components::Collider, Components::Movement>(
            LineDeletion());

        LOG_INFO("Initializing game systems");
    }

    void Game::_initSubscriptions()
    {
        Common::initEngineSubscriptions(this->_engine);
        this->_registerEventCollision(this->_engine);
        this->_registerEventSpawnBlock(this->_engine);
        this->_registerEventGameOver(this->_engine);
        this->_registerEventRotateCntClockwise(this->_engine);
        this->_registerEventRotateClockwise(this->_engine);
        this->_registerEventMove(this->_engine);
        this->_registerEventLineComplete(this->_engine);
        LOG_INFO("Initializing event subscriptions");
    }

    void Game::_initKeybinds()
    {
        const std::unordered_map<std::string, ActionBinding> actionBindings = getActionBindings();
        const std::vector<std::pair<std::string, std::string>> keyNames     = {
            {"KEY_ESCAPE", "ExitGame"}, {"KEY_LEFT", "MoveLeft"}, {"KEY_RIGHT", "MoveRight"}};

        for (const auto& keyName : keyNames) {
            int key = this->_graphic->stringtoKeyCode(keyName.first);

            auto it = actionBindings.find(keyName.second);
            if (it == actionBindings.end())
                throw std::runtime_error("Unknown action: " + keyName.second);
            const ActionBinding& action = it->second;

            if (action.onPress) {
                LOG_INFO("Adding action to key {} ({})", key, keyName.second);
                this->_graphic->addKeyMapping(key, action.onPress);
            }

            if (action.onRelease) {
                this->_graphic->addKeyReleasedMapping(key, action.onRelease);
            }
        }

        int keyZ                              = this->_graphic->stringtoKeyCode("KEY_W");
        const ActionBinding& Z_action_binding = ActionBinding{.onPress =
                                                                  [](Registry& registry) {
                                                                      registry.publish(EventRotateCntClockwise{true});
                                                                  },
                                                              .onRelease =
                                                                  [](Registry& registry) {
                                                                      registry.publish(EventRotateCntClockwise{false});
                                                                  }};
        if (Z_action_binding.onPress) {
            LOG_INFO("Adding action to keyZ {}", keyZ);
            this->_graphic->addKeyMapping(keyZ, Z_action_binding.onPress);
        }

        if (Z_action_binding.onRelease) {
            this->_graphic->addKeyReleasedMapping(keyZ, Z_action_binding.onRelease);
        }

        int keyA                              = this->_graphic->stringtoKeyCode("KEY_A");
        const ActionBinding& A_action_binding = ActionBinding{.onPress =
                                                                  [](Registry& registry) {
                                                                      registry.publish(EventRotateClockwise{true});
                                                                  },
                                                              .onRelease =
                                                                  [](Registry& registry) {
                                                                      registry.publish(EventRotateClockwise{false});
                                                                  }};
        if (A_action_binding.onPress) {
            LOG_INFO("Adding action to keyA {}", keyA);
            this->_graphic->addKeyMapping(keyA, A_action_binding.onPress);
        }

        if (A_action_binding.onRelease) {
            this->_graphic->addKeyReleasedMapping(keyA, A_action_binding.onRelease);
        }

        LOG_INFO("Initializing keybinds");
    }

    void Game::_initGraphic()
    {
        this->_graphic->initWindow(WIN_WIDTH, WIN_HEIGHT, "Tetris Game");
        const std::map<std::string, std::string>& sprites = {
            {"BLOCK", "assets/tetris-Sheet.png"},
        };
        const std::map<std::string, std::string>& fonts = {
            {"TETRIS_FONT", "assets/tetris-font.ttf"},
        };
        const std::map<std::string, std::string>& sounds = {
            {"MENU_SOUND", "assets/Tetris (GB) (17)-menu_sound.wav"},
            {"MOVE_PIECE", "assets/Tetris (GB) (18)-move_piece.wav"},
            {"ROTATE_PIECE", "assets/Tetris (GB) (19)-rotate_piece.wav"},
            {"LINE_CLEAR", "assets/Tetris (GB) (21)-line_clear.wav"},
            {"GAME_OVER", "assets/Tetris (GB) (25)-game_over.wav"},
            {"PIECE_LAND", "assets/Tetris (GB) (27)-piece_landed.wav"},
        };
        this->_resource_manager.get()->loadTexturesFromMap(sprites);
        this->_resource_manager.get()->loadFontsFromMap(fonts);
        this->_resource_manager.get()->loadSoundsFromMap(sounds);

        this->_offset_x = this->_graphic->getWindowSize().first / 2 - (BOARD_WIDTH * (WIDTH_BLOCK * BLOCK_SCALE)) / 2;
        this->_offset_y = this->_graphic->getWindowSize().second - (BOARD_HEIGHT * (HEIGHT_BLOCK * BLOCK_SCALE));

        // Test sound loading
        LOG_INFO("Testing sound system...");
        this->_graphic->setVolumeSound("PIECE_LAND", 1.0f);
        this->_graphic->playSound("PIECE_LAND");
        LOG_INFO("Sound test complete");

        LOG_INFO("Initializing graphics");
    }

} // namespace Tetris