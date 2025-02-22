#include "game.h"
#include <iostream>
#include <cctype>      // For std::tolower

void Game::startGame() {
    std::string command;
    std::cout << "You are in the starting position, type a command or type 'help' for help" << std::endl;

    isRunning = true;
    while (isRunning) {
        std::cout << "> ";
        std::getline(std::cin, command);
        for (char& c : command) {
            c = std::tolower(c);
        }
        processCommand(command);
        

    }
}

void Game::processCommand(std::string command) {
    if (command == "quit") {
        isRunning = false;;
    }
    else if (command == "help") {
        displayHelp();
    }
    else if (command == "look") {
        std::cout << "You are in the starting position" << std::endl;
    }
    else {
        std::cout << "Invalid command" << std::endl;
    }
}

void Game::displayHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  help  - Display this help message" << std::endl;
    std::cout << "  quit  - Quit the game" << std::endl;
    std::cout << "  look  - Look around" << std::endl;
}