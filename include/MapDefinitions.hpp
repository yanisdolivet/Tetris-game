/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** MapDefinitions
*/

#ifndef MAPDEFINITIONS_HPP_
#define MAPDEFINITIONS_HPP_

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define WIDTH_BLOCK 16.0f
#define HEIGHT_BLOCK WIDTH_BLOCK

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#define BLOCK_SCALE 1.0f

#define STARTING_POSITION_X (240.0f + 6 * (WIDTH_BLOCK * BLOCK_SCALE))
#define STARTING_POSITION_Y 216.0f

#define BLOCKNAME "BLOCK"

enum BlockColor
{
    YELLOW_BLOCK,
    GREEN_BLOCK,
    PURPLE_BLOCK,
    BLUE_BLOCK,
    ORANGE_BLOCK,
    PINK_BLOCK,
    PASTEL_BLUE_BLOCK,
    GREY_BLOCK,
    BROWN_BLOCK,
    PASTEL_GREEN_BLOCK
};

enum class TetrominoType
{
    I,
    J,
    L,
    O,
    S,
    T,
    Z
};

#define GAME_MENU "TETRIS_MENU"
#define GAME_PLAY "TETRIS_PLAY"

#endif /* !MAPDEFINITIONS_HPP_ */
