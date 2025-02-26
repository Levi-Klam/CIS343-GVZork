#include "game.h"
#include <iostream>
#include <cctype>  
#include "classes.hpp" 

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
        isRunning = false;
        exit();
    }
    else if (command == "help") {
        displayHelp();
    }
    else if (command == "look") {
        std::cout << curLocation << std::endl;
    }
    else if (
        command == "north" ||
        command == "east"  ||
        command == "south" ||
        command == "west"
    ) {
        go(command);
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
    std::cout << "  go [direction] - Move in a direction (north, south, east, west)" << std::endl;
}

void Game::go(std::string target) {
    auto locations = curLocation.get_locations();
    auto it = locations.find(target);

    if (it == locations.end()) {
        std::cout << "You cannot go that way." << std::endl;
        return; 
    }

    // std::cout << &it->second.get() << std::endl;
    std::cout << "You moved." << std::endl;
    curLocation = it->second; // Move to the new location
}

void Game::createWorld() {
    // Create Location objects on the heap
    Location* kirkoff = new Location("Kirkoff", "The central bus station for GVSU.");
    Location* stadium = new Location("Stadium", "The GVSU soccer field.");
    Location* housing = new Location("On Campus Housing", "GVSU's home to Lakers.");
    Location* library = new Location("Library", "The library located on the Allendale campus.");
    Location* levisHouse = new Location("Levis House", "A GVSU student's house.");
    Location* mackinac = new Location("Mackinac", "Home of GVSU computing.");

    locationPtrs.push_back(kirkoff);
    locationPtrs.push_back(stadium);
    locationPtrs.push_back(housing);
    locationPtrs.push_back(library);
    locationPtrs.push_back(levisHouse);
    locationPtrs.push_back(mackinac);

    // Define movement directions
    kirkoff->add_location("west", *library);
    kirkoff->add_location("south", *stadium);

    stadium->add_location("south", *housing);
    stadium->add_location("north", *kirkoff);

    housing->add_location("north", *stadium);

    library->add_location("east", *kirkoff);
    library->add_location("north", *mackinac);

    mackinac->add_location("east", *levisHouse);
    mackinac->add_location("south", *library);

    levisHouse->add_location("west", *mackinac);

    // Set starting location
    curLocation = *kirkoff; // Set curLocation to point to kirkoff
}

void Game::exit() {
    for (auto location : locationPtrs) {
        delete location;
    }
}

