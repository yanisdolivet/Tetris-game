/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** EventRotate
*/

#include <map>
#include <vector>

#include "Game.hpp"

namespace
{
    std::pair<int, int> getKick(TetrominoType type, int oldState, int newState, int testIdx)
    {
        // J, L, S, T, Z (CCW from 0 -> 3)
        if (type != TetrominoType::I && type != TetrominoType::O) {
            if (oldState == 0 && newState == 3) {
                // SRS JLSTZ 0->L: (0,0), (+1,0), (+1,-1), (0,+2), (+1,+2) (Y down)
                const std::vector<std::pair<int, int>> k = {{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}};
                return k[testIdx];
            }
            // ... Add other state transitions (1->0, 2->1, 3->2) ...
            // Fallback (basic rotation)
            return {0, 0};
        }

        // I Tetromino (CCW from 0 -> 3)
        if (type == TetrominoType::I) {
            if (oldState == 0 && newState == 3) { // 0 -> L
                // SRS I 0->L: (0,0), (-1,0), (+2,0), (-1,-2), (+2,+1) Y-Down
                const std::vector<std::pair<int, int>> k = {{0, 0}, {-1, 0}, {2, 0}, {-1, -2}, {2, 1}};
                return k[testIdx];
            }
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
                if (!tet.has_value() && !pos.has_value())
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
                for (auto& update : newPositions) {
                    positions[update.first] = update.second;
                    // Update Local Pos and Rotation State
                    auto& tet = tetrominos[update.first];
                    if (!tet.has_value())
                        return;
                    int oldX = tet.value().getLocalX();
                    tet.value().setLocalPos(tet.value().getLocalY(), -oldX); // Update local to new rotated local
                    tet.value().setRotationState(newRotation);
                }
                LOG_INFO("Rotated Piece CCW with Kick Test {}", test);
                return; // Success
            }
        }
        LOG_TRACE("Rotation Failed");
    });
}