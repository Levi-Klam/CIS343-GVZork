<<<<<<< HEAD
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

=======
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

>>>>>>> 1f1e89e0885b47046d1924395484d408c8f6fa77
#endif // GAME_H