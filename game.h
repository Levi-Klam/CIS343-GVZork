#ifndef GAME_H
#define GAME_H
#include <vector>
#include <string>

class Game {
private:
    std::string playerLocation;
    std::vector<std::string> playerInventory;
    std::vector<std::string> playerCommands;

public:
    void startGame();
};

#endif // GAME_H