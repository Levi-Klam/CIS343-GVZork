#ifndef GAME_H
#define GAME_H

// Created By: Levi Klamer, Mason Passinault, Blaze Lauer

#include <vector>
#include <string>
#include "classes.hpp"

class Game {
private:
    Location* curLocation;
    std::vector<Location*> locationPtrs;
    std::vector<Item> playerInventory;
    float wishgranterCalories;
    float playerCarryWeight;
    bool isRunning;
    std::unordered_map<std::string, std::function<void(const std::string&)>> commandMap; // Initialize the map for different command syntax.

public:
    void startGame();
    void processCommand(std::string command);
    void displayHelp();
    void go(std::string target);
    void createWorld();
    void take(std::string itemName);
    void drop(std::string itemName);
    void show_items();
    void talk(const std::string& npcName);
    void give(const std::string& itemName, const std::string& npcName);
    void teleport();
    void siphonGas();
    void read();
    void jump();
    void initializeCommands();
    void youWin();
    void exit();
};

#endif // GAME_H
