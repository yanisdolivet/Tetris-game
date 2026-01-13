/*
** EPITECH PROJECT, 2026
** Tetris-game
** File description:
** Game
*/

#pragma once

#include "Logs.hpp"

namespace Tetris {
    class Game {
        public:
            Game();
            ~Game();

            /*
            * @brief Initialize the game
            * @return void
            */
            void init();

            /*
            * @brief Run the game loop
            * @return void
            */
            void run();

        protected:
        private:
    };
}