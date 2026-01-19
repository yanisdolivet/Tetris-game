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
        this->_current_scene    = GAME_MENU;
    }

    Game::~Game()
    {
    }
    void Game::init()
    {
        this->_initScenes();
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

    void Game::_killAllEntities()
    {
        Registry& registry = this->_engine.getRegistry();

        auto& positions = registry.getComponents<Components::Position>();

        std::vector<Entity> entitiesToKill;
        for (auto [idx, pos] : IndexedZipper(positions)) {
            if (positions[idx].has_value()) {
                entitiesToKill.push_back(static_cast<Entity>(idx));
            }
        }

        // Kill all entities
        for (Entity entity : entitiesToKill) {
            registry.killEntity(entity);
            LOG_TRACE("Killed entity {}", static_cast<std::size_t>(entity));
        }

        LOG_INFO("Killed {} entities from the previous scene", entitiesToKill.size());
    }

    void Game::_initScenes()
    {
        std::map<std::string,
                 std::tuple<std::function<void()>, std::function<void(std::string)>, std::function<void()>>>
            scenes = {{GAME_PLAY,
                       {[this]() {
                            // Kill entities from previous scene if needed
                            this->_killAllEntities();
                        },
                        [this](std::string event) {
                            // Handle scene-specific events
                            this->_current_scene = GAME_PLAY;
                            this->_initSubscriptions();
                            LOG_INFO("Game Scene Start");
                        },
                        [this]() {
                            // Spawn entities for this scene
                            this->_createBoard(this->_engine.getRegistry());
                            this->_createMainText(this->_engine.getRegistry());
                        }}}};

        this->_scene_manager.loadSceneFromMap(scenes);
    }

    void Game::_initEntities()
    {
        Registry& registry = this->_engine.getRegistry();

        if (this->_current_scene == GAME_PLAY) {
            this->_createBoard(registry);
            this->_createMainText(registry);
        }
        else if (this->_current_scene == GAME_MENU) {
            this->_createMainText(registry);
            this->_createMenuText(registry);
        }

        LOG_INFO("All entities initialized");
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
        this->_engine.getRegistry().registerComponent<Components::Scale>();

        LOG_INFO("Initializing game components");
    }

    void Game::_initSystems()
    {
        // Animation System
        this->_engine.getRegistry()
            .addSystem<Components::DrawableComponent, Components::Position, Components::AnimationComponent,
                       Components::Parallax>(AnimationSystem());

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
        this->_engine.getRegistry().addSystem<Components::Position, Components::SpriteComponent>(
            BlockSpawner(this->_offset_y));

        // Line Deletion System
        this->_engine.getRegistry().addSystem<Components::Position, Components::Collider, Components::Movement>(
            LineDeletion());

        // Audio System
        this->_engine.getRegistry().addSystem<>(AudioSystem(this->_graphic));

        // Rendering System
        this->_engine.getRegistry().addSystem<Components::Position, Components::DrawableComponent>(
            RenderSystem(this->_graphic));

        LOG_INFO("Initializing game systems");
    }

    void Game::_initSubscriptions()
    {
        Common::initEngineSubscriptions(this->_engine, std::ref(this->_scene_manager));

        if (this->_current_scene == GAME_PLAY) {
            this->_registerEventCollision(this->_engine, this->_offset_y);
            this->_registerEventSpawnBlock(this->_engine);
            this->_registerEventGameOver(this->_engine);
            this->_registerEventRotateCntClockwise(this->_engine);
            this->_registerEventRotateClockwise(this->_engine);
            this->_registerEventMove(this->_engine);
            this->_registerEventLineComplete(this->_engine);
        }
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
        const ActionBinding& Z_action_binding = ActionBinding{.onPress = nullptr, .onRelease = [](Registry& registry) {
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
        const ActionBinding& A_action_binding = ActionBinding{.onPress = nullptr, .onRelease = [](Registry& registry) {
                                                                  registry.publish(EventRotateClockwise{false});
                                                              }};
        if (A_action_binding.onPress) {
            LOG_INFO("Adding action to keyA {}", keyA);
            this->_graphic->addKeyMapping(keyA, A_action_binding.onPress);
        }

        if (A_action_binding.onRelease) {
            this->_graphic->addKeyReleasedMapping(keyA, A_action_binding.onRelease);
        }

        int keySpace                              = this->_graphic->stringtoKeyCode("SPACE");
        const ActionBinding& SPACE_action_binding = ActionBinding{.onPress =
                                                                      [](Registry& registry) {
                                                                          registry.publish(EventChangeScene{GAME_PLAY});
                                                                      },
                                                                  .onRelease = nullptr};
        if (SPACE_action_binding.onPress) {
            LOG_INFO("Adding action to keySpace {}", keySpace);
            this->_graphic->addKeyMapping(keySpace, SPACE_action_binding.onPress);
        }

        if (SPACE_action_binding.onRelease) {
            this->_graphic->addKeyReleasedMapping(keySpace, SPACE_action_binding.onRelease);
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
        const std::map<std::string, std::string>& musics = {
            {"BASIC_BG_MUSIC", "assets/music-for-puzzle-game-146738.mp3"}};

        this->_resource_manager->loadTexturesFromMap(sprites);
        this->_resource_manager->loadFontsFromMap(fonts);
        this->_resource_manager->loadSoundsFromMap(sounds);
        this->_resource_manager->loadMusicsFromMap(musics);

        this->_offset_x = this->_graphic->getWindowSize().first / 2 - (BOARD_WIDTH * (WIDTH_BLOCK * BLOCK_SCALE)) / 2;
        this->_offset_y = this->_graphic->getWindowSize().second - (BOARD_HEIGHT * (HEIGHT_BLOCK * BLOCK_SCALE));

        // Start playing the music;
        this->_graphic->setVolumeMusic("BASIC_BG_MUSIC", 0.3f);
        this->_graphic->playMusic("BASIC_BG_MUSIC");

        LOG_INFO("Initializing graphics");
    }

} // namespace Tetris