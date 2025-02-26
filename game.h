#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "classes.hpp"

class Game {
private:
    Location curLocation;
    std::vector<Location*> locationPtrs;
    std::vector<std::string> playerInventory;
    bool isRunning;

public:
    void startGame();
    void processCommand(std::string command);
    void displayHelp();
    void go(std::string target);
    void createWorld();
    void exit();
};

#endif // GAME_H
