#ifndef GAME_H
#define GAME_H
#include <vector>
#include <string>

class Game {
private:
    std::string playerLocation;
    std::vector<std::string> playerInventory;
    bool isRunning;

public:
    void startGame();
    void processCommand(std::string command);
    void displayHelp();
};

#endif // GAME_H