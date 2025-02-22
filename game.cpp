#include "game.h"
#include <iostream>

void Game::startGame() {
    std::string command;
    std::cout << "You are in the starting position, type a command or type 'help' for help" << std::endl;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "quit") {
            break;
        }

        //processCommand(command);
    }
}