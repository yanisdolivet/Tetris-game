/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** LineDeletion
*/

#include "LineDeletion.hpp"

void LineDeletion::operator()(Registry& registry, double, SparseArray<Components::Position>& positions,
                              SparseArray<Components::Collider>& colliders) const
{
    std::vector<Entity> entities_to_delete;

    int row = (BOARD_HEIGHT - 3);
    for (auto [idx, pos, cold] : IndexedZipper(positions, colliders)) {
        if (cold.getType() != BLOCKNAME)
            continue;

        Entity entity = registry.entityFromIndex(idx);
        if (pos.getY() == row * (HEIGHT_BLOCK * BLOCK_SCALE) + STARTING_POSITION_Y) {
            entities_to_delete.push_back(entity);
        }
    }
    if (entities_to_delete.size() >= (BOARD_WIDTH - 2)) {
        registry.publish(EventLineComplete{row, entities_to_delete});
    }
    entities_to_delete.clear();
}
