/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** EventInput
*/

#include <map>
#include <vector>

#include "Game.hpp"

namespace
{
    /**
     * @brief Get the wall kick offset for SRS (Super Rotation System)
     *
     * @param type The tetromino type
     * @param oldState The old rotation state (0=spawn, 1=R, 2=2, 3=L)
     * @param newState The new rotation state
     * @param testIdx The kick test index (0-4)
     * @return std::pair<int, int> The kick offset (x, y) in grid units
     */
    std::pair<int, int> getKick(TetrominoType type, int oldState, int newState, int testIdx)
    {
        // O piece doesn't rotate
        if (type == TetrominoType::O) {
            return {0, 0};
        }

        // I Tetromino has its own kick table
        if (type == TetrominoType::I) {
            // Define all kick tables for I piece
            // Format: {x_offset, y_offset} where positive Y is DOWN
            static const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> iKicks = {
                // 0 -> R (clockwise)
                {{0, 1}, {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}}},
                // R -> 0 (counter-clockwise)
                {{1, 0}, {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}}},
                // R -> 2 (clockwise)
                {{1, 2}, {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}}},
                // 2 -> R (counter-clockwise)
                {{2, 1}, {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}}},
                // 2 -> L (clockwise)
                {{2, 3}, {{0, 0}, {2, 0}, {-1, 0}, {2, -1}, {-1, 2}}},
                // L -> 2 (counter-clockwise)
                {{3, 2}, {{0, 0}, {-2, 0}, {1, 0}, {-2, 1}, {1, -2}}},
                // L -> 0 (clockwise)
                {{3, 0}, {{0, 0}, {1, 0}, {-2, 0}, {1, 2}, {-2, -1}}},
                // 0 -> L (counter-clockwise)
                {{0, 3}, {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}}}};

            auto it = iKicks.find({oldState, newState});
            if (it != iKicks.end() && testIdx < static_cast<int>(it->second.size())) {
                return it->second[testIdx];
            }
            return {0, 0};
        }

        // J, L, S, T, Z pieces share the same kick table
        // Define all kick tables for JLSTZ pieces
        static const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> jlstzKicks = {
            // 0 -> R (clockwise)
            {{0, 1}, {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},
            // R -> 0 (counter-clockwise)
            {{1, 0}, {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}},
            // R -> 2 (clockwise)
            {{1, 2}, {{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}},
            // 2 -> R (counter-clockwise)
            {{2, 1}, {{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}},
            // 2 -> L (clockwise)
            {{2, 3}, {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}},
            // L -> 2 (counter-clockwise)
            {{3, 2}, {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},
            // L -> 0 (clockwise)
            {{3, 0}, {{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}},
            // 0 -> L (counter-clockwise)
            {{0, 3}, {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}}};

        auto it = jlstzKicks.find({oldState, newState});
        if (it != jlstzKicks.end() && testIdx < static_cast<int>(it->second.size())) {
            return it->second[testIdx];
        }
        return {0, 0};
    }
} // namespace

void Tetris::Game::_registerEventRotateCntClockwise(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventRotateCntClockwise>([this](const EventRotateCntClockwise&) {
        Registry& registry = this->_engine.getRegistry();
        auto& tetrominos   = registry.getComponents<Components::Tetromino>();
        auto& positions    = registry.getComponents<Components::Position>();
        auto& blockIds     = registry.getComponents<Components::BlockId>();
        auto& colliders    = registry.getComponents<Components::Collider>();

        int maxId = -1;
        for (auto [idx, id] : IndexedZipper(blockIds)) {
            if (id.getBlockId() > maxId)
                maxId = id.getBlockId();
        }
        if (maxId == -1)
            return;

        std::vector<Entity> pieceEntities;
        TetrominoType type = TetrominoType::O;
        int oldRotation    = 0;

        for (auto [idx, id, tet] : IndexedZipper(blockIds, tetrominos)) {
            if (id.getBlockId() == maxId) {
                pieceEntities.push_back(registry.entityFromIndex(idx));
                type        = tet.getType();
                oldRotation = tet.getRotationState();
            }
        }

        if (pieceEntities.empty() || type == TetrominoType::O)
            return; // O doesn't rotate

        int newRotation = (oldRotation + 3) % 4; // CCW: 0->3, 3->2, etc.

        // Perform Basic + 4 Kicks
        for (int test = 0; test < 5; ++test) {
            std::pair<int, int> kick = getKick(type, oldRotation, newRotation, test);
            float kickX              = kick.first * (WIDTH_BLOCK * BLOCK_SCALE);
            float kickY              = kick.second * (HEIGHT_BLOCK * BLOCK_SCALE);

            bool possible = true;
            std::vector<std::pair<Entity, Components::Position>> newPositions;

            for (Entity e : pieceEntities) {
                auto& tet = tetrominos[e];
                auto& pos = positions[e];

                // Rotate Local Position CCW (x, y) -> (y, -x) for +Y Down
                if (!tet.has_value() || !pos.has_value())
                    return;
                int oldLocX = tet.value().getLocalX();
                int oldLocY = tet.value().getLocalY();
                int newLocX = oldLocY;
                int newLocY = -oldLocX;

                // Calculate Pivot World Position
                float pivotWorldX = pos.value().getX() - (oldLocX * WIDTH_BLOCK * BLOCK_SCALE);
                float pivotWorldY = pos.value().getY() - (oldLocY * HEIGHT_BLOCK * BLOCK_SCALE);

                // Calculate New World Position
                float targetX = pivotWorldX + (newLocX * WIDTH_BLOCK * BLOCK_SCALE) + kickX;
                float targetY = pivotWorldY + (newLocY * HEIGHT_BLOCK * BLOCK_SCALE) + kickY;

                // Collision Check
                if (targetX < this->_offset_x ||
                    targetX >= this->_offset_x + (BOARD_WIDTH * WIDTH_BLOCK * BLOCK_SCALE) ||
                    targetY >= this->_offset_y + (BOARD_HEIGHT * HEIGHT_BLOCK * BLOCK_SCALE)) {
                    possible = false;
                    break;
                }

                // Collision with Other Blocks
                for (auto [cIdx, cPos, cCol] : IndexedZipper(positions, colliders)) {
                    bool isSelf = false;
                    for (Entity active : pieceEntities)
                        if (cIdx == static_cast<size_t>(active))
                            isSelf = true;
                    if (isSelf)
                        continue;

                    // Simple AABB or Point check
                    if (std::abs(cPos.getX() - targetX) < (WIDTH_BLOCK * BLOCK_SCALE) / 2 &&
                        std::abs(cPos.getY() - targetY) < (HEIGHT_BLOCK * BLOCK_SCALE) / 2) {
                        possible = false;
                        break;
                    }
                }

                if (!possible)
                    break;
                newPositions.push_back({e, Components::Position{targetX, targetY}});
            }

            if (possible) {
                // Apply Rotation
                for (size_t i = 0; i < newPositions.size(); ++i) {
                    auto& update = newPositions[i];
                    Entity e     = update.first;

                    // Update position
                    registry.getSpecificComponent<Components::Position>(e).setX(update.second.getX());
                    registry.getSpecificComponent<Components::Position>(e).setY(update.second.getY());

                    // Update local position and rotation state
                    auto& tet   = registry.getSpecificComponent<Components::Tetromino>(e);
                    int oldLocX = tet.getLocalX();
                    int oldLocY = tet.getLocalY();

                    // CCW rotation: (x, y) -> (y, -x)
                    int newLocX = oldLocY;
                    int newLocY = -oldLocX;

                    tet.setLocalPos(newLocX, newLocY);
                    tet.setRotationState(newRotation);
                }
                LOG_INFO("Rotated Piece CCW with Kick Test {}", test);
                return; // Success
            }
        }
        LOG_TRACE("Rotation Failed");
    });
}

void Tetris::Game::_registerEventRotateClockwise(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventRotateClockwise>([this](const EventRotateClockwise&) {
        Registry& registry = this->_engine.getRegistry();
        auto& tetrominos   = registry.getComponents<Components::Tetromino>();
        auto& positions    = registry.getComponents<Components::Position>();
        auto& blockIds     = registry.getComponents<Components::BlockId>();
        auto& colliders    = registry.getComponents<Components::Collider>();

        int maxId = -1;
        for (auto [idx, id] : IndexedZipper(blockIds)) {
            if (id.getBlockId() > maxId)
                maxId = id.getBlockId();
        }
        if (maxId == -1)
            return;

        std::vector<Entity> pieceEntities;
        TetrominoType type = TetrominoType::O;
        int oldRotation    = 0;

        for (auto [idx, id, tet] : IndexedZipper(blockIds, tetrominos)) {
            if (id.getBlockId() == maxId) {
                pieceEntities.push_back(registry.entityFromIndex(idx));
                type        = tet.getType();
                oldRotation = tet.getRotationState();
            }
        }

        if (pieceEntities.empty() || type == TetrominoType::O)
            return; // O doesn't rotate

        int newRotation = (oldRotation + 1) % 4; // CW: 0->1, 1->2, etc.

        // Perform Basic + 4 Kicks
        for (int test = 0; test < 5; ++test) {
            std::pair<int, int> kick = getKick(type, oldRotation, newRotation, test);
            float kickX              = kick.first * (WIDTH_BLOCK * BLOCK_SCALE);
            float kickY              = kick.second * (HEIGHT_BLOCK * BLOCK_SCALE);

            bool possible = true;
            std::vector<std::pair<Entity, Components::Position>> newPositions;

            for (Entity e : pieceEntities) {
                auto& tet = tetrominos[e];
                auto& pos = positions[e];

                // Rotate Local Position CW (x, y) -> (-y, x) for +Y Down
                if (!tet.has_value() || !pos.has_value())
                    return;
                int oldLocX = tet.value().getLocalX();
                int oldLocY = tet.value().getLocalY();
                int newLocX = -oldLocY;
                int newLocY = oldLocX;

                // Calculate Pivot World Position
                float pivotWorldX = pos.value().getX() - (oldLocX * WIDTH_BLOCK * BLOCK_SCALE);
                float pivotWorldY = pos.value().getY() - (oldLocY * HEIGHT_BLOCK * BLOCK_SCALE);

                // Calculate New World Position
                float targetX = pivotWorldX + (newLocX * WIDTH_BLOCK * BLOCK_SCALE) + kickX;
                float targetY = pivotWorldY + (newLocY * HEIGHT_BLOCK * BLOCK_SCALE) + kickY;

                // Collision Check
                if (targetX < this->_offset_x ||
                    targetX >= this->_offset_x + (BOARD_WIDTH * WIDTH_BLOCK * BLOCK_SCALE) ||
                    targetY >= this->_offset_y + (BOARD_HEIGHT * HEIGHT_BLOCK * BLOCK_SCALE)) {
                    possible = false;
                    break;
                }

                // Collision with Other Blocks
                for (auto [cIdx, cPos, cCol] : IndexedZipper(positions, colliders)) {
                    bool isSelf = false;
                    for (Entity active : pieceEntities)
                        if (cIdx == static_cast<size_t>(active))
                            isSelf = true;
                    if (isSelf)
                        continue;

                    // Simple AABB or Point check
                    if (std::abs(cPos.getX() - targetX) < (WIDTH_BLOCK * BLOCK_SCALE) / 2 &&
                        std::abs(cPos.getY() - targetY) < (HEIGHT_BLOCK * BLOCK_SCALE) / 2) {
                        possible = false;
                        break;
                    }
                }

                if (!possible)
                    break;
                newPositions.push_back({e, Components::Position{targetX, targetY}});
            }
            if (possible) {
                // Apply Rotation
                for (size_t i = 0; i < newPositions.size(); ++i) {
                    auto& update = newPositions[i];
                    Entity e     = update.first;

                    // Update position
                    registry.getSpecificComponent<Components::Position>(e).setX(update.second.getX());
                    registry.getSpecificComponent<Components::Position>(e).setY(update.second.getY());

                    // Update local position and rotation state
                    auto& tet   = registry.getSpecificComponent<Components::Tetromino>(e);
                    int oldLocX = tet.getLocalX();
                    int oldLocY = tet.getLocalY();

                    // CW rotation: (x, y) -> (-y, x)
                    int newLocX = -oldLocY;
                    int newLocY = oldLocX;

                    tet.setLocalPos(newLocX, newLocY);
                    tet.setRotationState(newRotation);
                }
                LOG_INFO("Rotated Piece CW with Kick Test {}", test);
                return; // Success
            }
        }
        LOG_TRACE("Rotation Failed");
    });
}

void Tetris::Game::_registerEventMove(GameEngine::Core& engine)
{
    engine.getRegistry().subscribe<EventLeft>([this](const EventLeft& event) {
        Registry& registry = this->_engine.getRegistry();
        auto& positions    = registry.getComponents<Components::Position>();
        auto& blockIds     = registry.getComponents<Components::BlockId>();
        auto& colliders    = registry.getComponents<Components::Collider>();

        int maxId = -1;
        for (auto [idx, id] : IndexedZipper(blockIds)) {
            if (id.getBlockId() > maxId)
                maxId = id.getBlockId();
        }
        if (maxId == -1)
            return;

        std::vector<Entity> pieceEntities;

        for (auto [idx, id] : IndexedZipper(blockIds)) {
            if (id.getBlockId() == maxId) {
                pieceEntities.push_back(registry.entityFromIndex(idx));
            }
        }

        if (pieceEntities.empty())
            return;

        float moveX = -WIDTH_BLOCK * BLOCK_SCALE;

        // Check if move is possible
        bool possible = true;
        for (Entity e : pieceEntities) {
            auto& pos     = positions[e];
            float targetX = pos.value().getX() + moveX;

            // Boundary Check
            if (targetX < (this->_offset_x - (WIDTH_BLOCK * BLOCK_SCALE))) {
                possible = false;
                break;
            }

            // Collision with Other Blocks
            for (auto [cIdx, cPos, cCol] : IndexedZipper(positions, colliders)) {
                bool isSelf = false;
                for (Entity active : pieceEntities)
                    if (cIdx == static_cast<size_t>(active))
                        isSelf = true;
                if (isSelf)
                    continue;

                // Simple AABB or Point check
                if (std::abs(cPos.getX() - targetX) < (WIDTH_BLOCK * BLOCK_SCALE) / 2 &&
                    std::abs(cPos.getY() - pos.value().getY()) < (HEIGHT_BLOCK * BLOCK_SCALE) / 2) {
                    possible = false;
                    break;
                }
            }

            if (!possible)
                break;
        }

        if (possible) {
            // Apply Move
            for (Entity e : pieceEntities) {
                auto& pos = registry.getSpecificComponent<Components::Position>(e);
                pos.setX(pos.getX() + moveX);
            }
            LOG_INFO("Moved Piece Left");
        }
    });

    engine.getRegistry().subscribe<EventRight>([this](const EventRight& event) {
        Registry& registry = this->_engine.getRegistry();
        auto& positions    = registry.getComponents<Components::Position>();
        auto& blockIds     = registry.getComponents<Components::BlockId>();
        auto& colliders    = registry.getComponents<Components::Collider>();

        int maxId = -1;
        for (auto [idx, id] : IndexedZipper(blockIds)) {
            if (id.getBlockId() > maxId)
                maxId = id.getBlockId();
        }
        if (maxId == -1)
            return;

        std::vector<Entity> pieceEntities;

        for (auto [idx, id] : IndexedZipper(blockIds)) {
            if (id.getBlockId() == maxId) {
                pieceEntities.push_back(registry.entityFromIndex(idx));
            }
        }

        if (pieceEntities.empty())
            return;

        float moveX = WIDTH_BLOCK * BLOCK_SCALE;

        // Check if move is possible
        bool possible = true;
        for (Entity e : pieceEntities) {
            auto& pos     = positions[e];
            float targetX = pos.value().getX() + moveX;

            // Boundary Check
            if (targetX > (this->_offset_x + (BOARD_WIDTH * (WIDTH_BLOCK * BLOCK_SCALE)))) {
                possible = false;
                break;
            }

            // Collision with Other Blocks
            for (auto [cIdx, cPos, cCol] : IndexedZipper(positions, colliders)) {
                bool isSelf = false;
                for (Entity active : pieceEntities)
                    if (cIdx == static_cast<size_t>(active))
                        isSelf = true;
                if (isSelf)
                    continue;

                // Simple AABB or Point check
                if (std::abs(cPos.getX() - targetX) < (WIDTH_BLOCK * BLOCK_SCALE) / 2 &&
                    std::abs(cPos.getY() - pos.value().getY()) < (HEIGHT_BLOCK * BLOCK_SCALE) / 2) {
                    possible = false;
                    break;
                }
            }

            if (!possible)
                break;
        }

        if (possible) {
            // Apply Move
            for (Entity e : pieceEntities) {
                auto& pos = registry.getSpecificComponent<Components::Position>(e);
                pos.setX(pos.getX() + moveX);
            }
            LOG_INFO("Moved Piece Left");
        }
    });
}