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

    // Create item objects
    Item* book = new Item("Book", "A book for reading.", 0, 0.5);
    Item* backpack = new Item("Backpack", "A backpack for carrying items.", 0, 2.0);
    Item* levisJournal = new Item("Levi's Journal", "A journal with notes about C++.", 0, 1.0);
    Item* busPass = new Item("Bus Pass", "A bus pass for the Laker Line.", 0, 0.1);
    Item* cake = new Item("Chocolate Cake", "Must've been a birthday party.", 150, 5.0);
    Item* nuggies = new Item("Nuggies", "6 piece nugget.", 50, 1.0);
    Item* granolaBar = new Item("Granola Bar", "A light snack.", 25, 0.5);
    Item* bread = new Item("Bread", "A loaf of bread.", 50, 1.0);
    Item* fish = new Item("Fish", "A fish.", 100, 3.0);
    Item* helmet = new Item("Helmet", "A helmet for protection.", 0, 5.0);

    // Add items to locations
    stadium->add_item(*helmet);
    levisHouse->add_item(*levisJournal);
    library->add_item(*book);
    housing->add_item(*backpack);
    kirkoff->add_item(*busPass);
    housing->add_item(*cake);
    mackinac->add_item(*nuggies);
    levisHouse->add_item(*granolaBar);
    library->add_item(*bread);
    stadium->add_item(*fish);

    // Create NPC objects
    NPC* levi = new NPC("Levi", "A GVSU student who loves C++.");
    NPC* busDriver = new NPC("Bus Driver", "The driver of the Laker Line.");
    NPC* librarian = new NPC("Librarian", "The librarian at the GVSU library.");

    // Add NPCs to locations
    library->add_npc(*librarian);
    kirkoff->add_npc(*busDriver);
    levisHouse->add_npc(*levi);
}

void Game::exit() {
    for (auto location : locationPtrs) {
        delete location;
    }
}

