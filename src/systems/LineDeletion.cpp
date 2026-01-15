/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** LineDeletion
*/

#include "LineDeletion.hpp"

void LineDeletion::operator()(Registry& registry, double, SparseArray<Components::Position>& positions,
                              SparseArray<Components::Collider>& colliders,
                              SparseArray<Components::Movement>& movements) const
{
    std::vector<Entity> entities_to_delete;

    int row = BOARD_HEIGHT;
    for (; row >= 0; --row) {
        for (auto [idx, pos, cold, mov] : IndexedZipper(positions, colliders, movements)) {
            // Ignore non-block entities and moving blocks
            if (cold.getType() != BLOCKNAME || mov.getVertical() != 0)
                continue;

            Entity entity = registry.entityFromIndex(idx);
            float epsilon = HEIGHT_BLOCK * BLOCK_SCALE / 2.0f;
            if (std::abs(pos.getY() - (row * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y)) < epsilon) {
                entities_to_delete.push_back(entity);
            }
        }
        if (entities_to_delete.size() >= (BOARD_WIDTH - 2)) {
            registry.publish(EventLineComplete{row, entities_to_delete});
        }
        entities_to_delete.clear();
    }
}
