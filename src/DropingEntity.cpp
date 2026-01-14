/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** DropingEntity
*/

#include "DropingEntity.hpp"

namespace Tetris
{
    DropingEntity::DropingEntity(Registry& registry) : _registry(registry)
    {
    }

    DropingEntity::~DropingEntity()
    {
    }

    std::vector<Entity> DropingEntity::generateL(int row, SparseArray<Components::Position>& positions,
                                                 SparseArray<Components::Movement>& movements,
                                                 SparseArray<Components::BlockId>& blockids) const
    {
        int max_row = BOARD_WIDTH - 4;
        if (row > max_row) {
            row = max_row;
        }
        std::vector<Entity> entities;
        std::vector<std::pair<int, int>> posoffset = {{0, 0}, {0, 1}, {0, 2}, {1, 2}};
        for (const auto& [colOffset, rowOffset] : posoffset) {
            std::pair<float, float> sppos = {
                static_cast<float>((row + colOffset) * (WIDTH_BLOCK * BLOCK_SCALE) + STARTING_POSITION_X),
                static_cast<float>(rowOffset * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y)};

            for (auto&& [idx, pos, mv, blid] : IndexedZipper(positions, movements, blockids)) {
                // Check if there's a stationary block at the spawn position
                if (mv.getVertical() == 0) {
                    float blockX = pos.getX();
                    float blockY = pos.getY();
                    float spawnX = sppos.first;
                    float spawnY = sppos.second;

                    // Check if positions overlap (within block size tolerance)
                    bool xOverlap = std::abs(blockX - spawnX) < (WIDTH_BLOCK * BLOCK_SCALE);
                    bool yOverlap = std::abs(blockY - spawnY) < (HEIGHT_BLOCK * BLOCK_SCALE);

                    if (xOverlap && yOverlap) {
                        LOG_FATAL("Lose condition met: Block collision at spawn position for entity {}", idx);
                        _registry.publish(EventGameOver{});
                        return {};
                    }
                }
            }
            Entity entity = _registry.spawnEntity();
            _registry.getComponents<Components::Position>().insertAt(entity,
                                                                     Components::Position{sppos.first, sppos.second});
            entities.push_back(entity);
        }
        return entities;
    }

    std::vector<Entity> DropingEntity::generateO(int row, SparseArray<Components::Position>& positions,
                                                 SparseArray<Components::Movement>& movements,
                                                 SparseArray<Components::BlockId>& blockids) const
    {
        int max_row = BOARD_WIDTH - 4;
        if (row > max_row) {
            row = max_row;
        }
        std::vector<Entity> entities;
        for (int i = 0; i < 4; ++i) {
            std::pair<float, float> sppos = {
                static_cast<float>((row + (i % 2)) * (WIDTH_BLOCK * BLOCK_SCALE) + STARTING_POSITION_X),
                static_cast<float>((i / 2) * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y)};

            for (auto&& [idx, pos, mv, blid] : IndexedZipper(positions, movements, blockids)) {
                // Check if there's a stationary block at the spawn position
                if (mv.getVertical() == 0) {
                    float blockX = pos.getX();
                    float blockY = pos.getY();
                    float spawnX = sppos.first;
                    float spawnY = sppos.second;

                    // Check if positions overlap (within block size tolerance)
                    bool xOverlap = std::abs(blockX - spawnX) < (WIDTH_BLOCK * BLOCK_SCALE);
                    bool yOverlap = std::abs(blockY - spawnY) < (HEIGHT_BLOCK * BLOCK_SCALE);

                    if (xOverlap && yOverlap) {
                        LOG_FATAL("Lose condition met: Block collision at spawn position for entity {}", idx);
                        _registry.publish(EventGameOver{});
                        return {};
                    }
                }
            }
            Entity entity = _registry.spawnEntity();
            _registry.getComponents<Components::Position>().insertAt(entity,
                                                                     Components::Position{sppos.first, sppos.second});
            entities.push_back(entity);
        }
        return entities;
    }

    std::vector<Entity> DropingEntity::generateI(int row, SparseArray<Components::Position>& positions,
                                                 SparseArray<Components::Movement>& movements,
                                                 SparseArray<Components::BlockId>& blockids) const
    {
        std::vector<Entity> entities;
        for (int i = 0; i < 4; ++i) {
            std::pair<float, float> sppos = {
                static_cast<float>(row * (WIDTH_BLOCK * BLOCK_SCALE) + STARTING_POSITION_X),
                static_cast<float>(i * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y)};

            for (auto&& [idx, pos, mv, blid] : IndexedZipper(positions, movements, blockids)) {
                // Check if there's a stationary block at the spawn position
                if (mv.getVertical() == 0) {
                    float blockX = pos.getX();
                    float blockY = pos.getY();
                    float spawnX = sppos.first;
                    float spawnY = sppos.second;

                    // Check if positions overlap (within block size tolerance)
                    bool xOverlap = std::abs(blockX - spawnX) < (WIDTH_BLOCK * BLOCK_SCALE);
                    bool yOverlap = std::abs(blockY - spawnY) < (HEIGHT_BLOCK * BLOCK_SCALE);

                    if (xOverlap && yOverlap) {
                        LOG_FATAL("Lose condition met: Block collision at spawn position for entity {}", idx);
                        _registry.publish(EventGameOver{});
                        return {};
                    }
                }
            }
            Entity entity = _registry.spawnEntity();
            _registry.getComponents<Components::Position>().insertAt(entity,
                                                                     Components::Position{sppos.first, sppos.second});
            entities.push_back(entity);
        }
        return entities;
    }

    std::vector<Entity> DropingEntity::generateS(int row, SparseArray<Components::Position>& positions,
                                                 SparseArray<Components::Movement>& movements,
                                                 SparseArray<Components::BlockId>& blockids) const
    {
        int max_row = BOARD_WIDTH - 5;
        if (row > max_row) {
            row = max_row;
        }
        std::vector<Entity> entities;
        std::vector<std::pair<int, int>> posoffset = {{1, 0}, {2, 0}, {0, 1}, {1, 1}};
        for (const auto& [colOffset, rowOffset] : posoffset) {
            std::pair<float, float> sppos = {
                static_cast<float>((row + colOffset) * (WIDTH_BLOCK * BLOCK_SCALE) + STARTING_POSITION_X),
                static_cast<float>(rowOffset * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y)};

            for (auto&& [idx, pos, mv, blid] : IndexedZipper(positions, movements, blockids)) {
                // Check if there's a stationary block at the spawn position
                if (mv.getVertical() == 0) {
                    float blockX = pos.getX();
                    float blockY = pos.getY();
                    float spawnX = sppos.first;
                    float spawnY = sppos.second;

                    // Check if positions overlap (within block size tolerance)
                    bool xOverlap = std::abs(blockX - spawnX) < (WIDTH_BLOCK * BLOCK_SCALE);
                    bool yOverlap = std::abs(blockY - spawnY) < (HEIGHT_BLOCK * BLOCK_SCALE);

                    if (xOverlap && yOverlap) {
                        LOG_FATAL("Lose condition met: Block collision at spawn position for entity {}", idx);
                        _registry.publish(EventGameOver{});
                        return {};
                    }
                }
            }
            Entity entity = _registry.spawnEntity();
            _registry.getComponents<Components::Position>().insertAt(entity,
                                                                     Components::Position{sppos.first, sppos.second});
            entities.push_back(entity);
        }
        return entities;
    }

    std::vector<Entity> DropingEntity::generateZ(int row, SparseArray<Components::Position>& positions,
                                                 SparseArray<Components::Movement>& movements,
                                                 SparseArray<Components::BlockId>& blockids) const
    {
        int max_row = BOARD_WIDTH - 5;
        if (row > max_row) {
            row = max_row;
        }
        std::vector<Entity> entities;
        std::vector<std::pair<int, int>> posoffset = {{0, 0}, {1, 0}, {1, 1}, {2, 1}};
        for (const auto& [colOffset, rowOffset] : posoffset) {
            std::pair<float, float> sppos = {
                static_cast<float>((row + colOffset) * (WIDTH_BLOCK * BLOCK_SCALE) + STARTING_POSITION_X),
                static_cast<float>(rowOffset * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y)};

            for (auto&& [idx, pos, mv, blid] : IndexedZipper(positions, movements, blockids)) {
                // Check if there's a stationary block at the spawn position
                if (mv.getVertical() == 0) {
                    float blockX = pos.getX();
                    float blockY = pos.getY();
                    float spawnX = sppos.first;
                    float spawnY = sppos.second;

                    // Check if positions overlap (within block size tolerance)
                    bool xOverlap = std::abs(blockX - spawnX) < (WIDTH_BLOCK * BLOCK_SCALE);
                    bool yOverlap = std::abs(blockY - spawnY) < (HEIGHT_BLOCK * BLOCK_SCALE);

                    if (xOverlap && yOverlap) {
                        LOG_FATAL("Lose condition met: Block collision at spawn position for entity {}", idx);
                        _registry.publish(EventGameOver{});
                        return {};
                    }
                }
            }
            Entity entity = _registry.spawnEntity();
            _registry.getComponents<Components::Position>().insertAt(entity,
                                                                     Components::Position{sppos.first, sppos.second});
            entities.push_back(entity);
        }
        return entities;
    }

    std::vector<Entity> DropingEntity::generateJ(int row, SparseArray<Components::Position>& positions,
                                                 SparseArray<Components::Movement>& movements,
                                                 SparseArray<Components::BlockId>& blockids) const
    {
        int max_row = BOARD_WIDTH - 4;
        if (row > max_row) {
            row = max_row;
        }
        std::vector<Entity> entities;
        std::vector<std::pair<int, int>> posoffset = {{1, 0}, {1, 1}, {1, 2}, {0, 2}};
        for (const auto& [colOffset, rowOffset] : posoffset) {
            std::pair<float, float> sppos = {
                static_cast<float>((row + colOffset) * (WIDTH_BLOCK * BLOCK_SCALE) + STARTING_POSITION_X),
                static_cast<float>(rowOffset * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y)};

            for (auto&& [idx, pos, mv, blid] : IndexedZipper(positions, movements, blockids)) {
                // Check if there's a stationary block at the spawn position
                if (mv.getVertical() == 0) {
                    float blockX = pos.getX();
                    float blockY = pos.getY();
                    float spawnX = sppos.first;
                    float spawnY = sppos.second;

                    // Check if positions overlap (within block size tolerance)
                    bool xOverlap = std::abs(blockX - spawnX) < (WIDTH_BLOCK * BLOCK_SCALE);
                    bool yOverlap = std::abs(blockY - spawnY) < (HEIGHT_BLOCK * BLOCK_SCALE);

                    if (xOverlap && yOverlap) {
                        LOG_FATAL("Lose condition met: Block collision at spawn position for entity {}", idx);
                        _registry.publish(EventGameOver{});
                        return {};
                    }
                }
            }
            Entity entity = _registry.spawnEntity();
            _registry.getComponents<Components::Position>().insertAt(entity,
                                                                     Components::Position{sppos.first, sppos.second});
            entities.push_back(entity);
        }
        return entities;
    }

    std::vector<Entity> DropingEntity::generateT(int row, SparseArray<Components::Position>& positions,
                                                 SparseArray<Components::Movement>& movements,
                                                 SparseArray<Components::BlockId>& blockids) const
    {
        int max_row = BOARD_WIDTH - 5;
        if (row > max_row) {
            row = max_row;
        }
        std::vector<Entity> entities;
        std::vector<std::pair<int, int>> posoffset = {{0, 0}, {1, 0}, {2, 0}, {1, 1}};
        for (const auto& [colOffset, rowOffset] : posoffset) {
            std::pair<float, float> sppos = {
                static_cast<float>((row + colOffset) * (WIDTH_BLOCK * BLOCK_SCALE) + STARTING_POSITION_X),
                static_cast<float>(rowOffset * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y)};

            for (auto&& [idx, pos, mv, blid] : IndexedZipper(positions, movements, blockids)) {
                // Check if there's a stationary block at the spawn position
                if (mv.getVertical() == 0) {
                    float blockX = pos.getX();
                    float blockY = pos.getY();
                    float spawnX = sppos.first;
                    float spawnY = sppos.second;

                    // Check if positions overlap (within block size tolerance)
                    bool xOverlap = std::abs(blockX - spawnX) < (WIDTH_BLOCK * BLOCK_SCALE);
                    bool yOverlap = std::abs(blockY - spawnY) < (HEIGHT_BLOCK * BLOCK_SCALE);

                    if (xOverlap && yOverlap) {
                        LOG_FATAL("Lose condition met: Block collision at spawn position for entity {}", idx);
                        _registry.publish(EventGameOver{});
                        return {};
                    }
                }
            }
            Entity entity = _registry.spawnEntity();

            _registry.getComponents<Components::Position>().insertAt(entity,
                                                                     Components::Position{sppos.first, sppos.second});
            entities.push_back(entity);
        }
        return entities;
    }

    void DropingEntity::generateBlock(BlockColor color, int row, std::string blockType)
    {
        using BlockFunc = std::vector<Entity> (DropingEntity::*)(int, SparseArray<Components::Position>&,
                                                                 SparseArray<Components::Movement>&,
                                                                 SparseArray<Components::BlockId>&) const;

        static const std::unordered_map<std::string, BlockFunc> handlers = {
            {"L", &DropingEntity::generateL}, {"O", &DropingEntity::generateO}, {"I", &DropingEntity::generateI},
            {"S", &DropingEntity::generateS}, {"Z", &DropingEntity::generateZ}, {"J", &DropingEntity::generateJ},
            {"T", &DropingEntity::generateT}};

        for (const auto& [key, func] : handlers) {
            if (key == blockType) {
                auto& blockIds               = _registry.getComponents<Components::BlockId>();
                auto& movements              = _registry.getComponents<Components::Movement>();
                auto& positions              = _registry.getComponents<Components::Position>();
                std::vector<Entity> entities = (this->*func)(row, positions, movements, blockIds);

                int block_id = this->_currentBlockId;
                for (auto& entity : entities) {
                    GameEngine::Math::Rectangle source_rect = {WIDTH_BLOCK * static_cast<int>(color), 0.0f, WIDTH_BLOCK,
                                                               HEIGHT_BLOCK};
                    GameEngine::Math::Vector2 scale         = {BLOCK_SCALE, BLOCK_SCALE};
                    int layer                               = 1;
                    _registry.getComponents<Components::DrawableComponent>().insertAt(
                        entity, Components::DrawableComponent{source_rect, true, scale, layer});

                    _registry.getComponents<Components::SpriteComponent>().insertAt(
                        entity, Components::SpriteComponent{"BLOCK", layer});

                    _registry.getComponents<Components::Movement>().insertAt(entity, Components::Movement{1, 0});

                    _registry.getComponents<Components::Velocity>().insertAt(entity, Components::Velocity{});

                    _registry.getComponents<Components::Speed>().insertAt(entity, Components::Speed{1});

                    _registry.getComponents<Components::Collider>().insertAt(
                        entity, Components::Collider{static_cast<int>(WIDTH_BLOCK * scale.x),
                                                     static_cast<int>(HEIGHT_BLOCK * scale.y), "BLOCK"});

                    _registry.getComponents<Components::Collider>().insertAt(
                        entity, Components::Collider{static_cast<int>(WIDTH_BLOCK * scale.x),
                                                     static_cast<int>(HEIGHT_BLOCK * scale.y), "BLOCK"});

                    LOG_INFO("Assigned Block ID {}", block_id);
                    LOG_INFO("For entity {}", static_cast<int>(entity));
                    _registry.getComponents<Components::BlockId>().insertAt(entity, Components::BlockId(block_id));
                }
                this->_currentBlockId++;
                break;
            }
        }
    }

    void DropingEntity::spawnBlock()
    {
        BlockColor color                    = static_cast<BlockColor>(std::rand() % 7);
        int row                             = std::rand() % (BOARD_WIDTH - 2);
        std::vector<std::string> blockTypes = {"L", "O", "I", "S", "Z", "J", "T"};
        std::string blockType               = blockTypes[std::rand() % blockTypes.size()];

        LOG_INFO("Spawning block of color {} at row {}", static_cast<int>(color), row);
        this->generateBlock(color, row, blockType);
    }

} // namespace Tetris