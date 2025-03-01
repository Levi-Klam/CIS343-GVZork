// Created By: Levi Klamer, Mason Passinault, Blaze Lauer


#include "game.h"
#include <iostream>
#include <cctype>    // for std::tolower
#include <algorithm> // for std::find_if
#include <random>   // for teleporting the player
#include <unordered_map>
#include <functional>

#include "classes.hpp"

void Game::startGame() {
    initializeCommands(); // Initialize the command map

    std::string command;
    std::cout << "You wake up from a bad dream where you were late for class. Thankfully, you don't have classes anymore, because the world ended."  << std::endl;
    std::cout << "There's a monolithic rock in front of you, urging you to speak to it." << std::endl;
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

void Game::initializeCommands() {
    // Copilot assisted with the framework for the command map, using unordered_map and std::function
    commandMap["quit"] = [this](const std::string&) { isRunning = false; exit(); };
    commandMap["exit"] = [this](const std::string&) { isRunning = false; exit(); };
    commandMap["help"] = [this](const std::string&) { displayHelp(); };
    commandMap["display help"] = [this](const std::string&) { displayHelp(); };
    commandMap["look"] = [this](const std::string&) { std::cout << *curLocation << std::endl; };
    commandMap["go"] = [this](const std::string& arg) { go(arg); };
    commandMap["proceed"] = [this](const std::string& arg) { go(arg); };
    commandMap["walk"] = [this](const std::string& arg) { go(arg); };
    commandMap["pickup"] = [this](const std::string& arg) { take(arg); };
    commandMap["take"] = [this](const std::string& arg) { take(arg); };
    commandMap["grab"] = [this](const std::string& arg) { take(arg); };
    commandMap["drop"] = [this](const std::string& arg) { drop(arg); };
    commandMap["backpack"] = [this](const std::string&) { show_items(); };
    commandMap["inventory"] = [this](const std::string&) { show_items(); };
    commandMap["talk"] = [this](const std::string& arg) { 
        // Grab anything after the LAST space, so players can input "talk to __" or "talk with ___"
        size_t lastSpacePos = arg.find_last_of(' ');
        std::string npcName = (lastSpacePos == std::string::npos) ? arg : arg.substr(lastSpacePos + 1);
        talk(npcName); 
    };
    commandMap["speak"] = [this](const std::string& arg) { 
        size_t lastSpacePos = arg.find_last_of(' ');
        std::string npcName = (lastSpacePos == std::string::npos) ? arg : arg.substr(lastSpacePos + 1);
        talk(npcName); 
    };
    commandMap["give"] = [this](const std::string& arg) {
        size_t spacePos = arg.find(' ');
        if (spacePos != std::string::npos) {
            std::string npcName = arg.substr(0, spacePos);
            std::string itemName = arg.substr(spacePos + 1);
            give(npcName, itemName);
        } else {
            std::cout << "Invalid give command format. Use 'give [npc] [item]'." << std::endl;
        }
    };
}

void Game::processCommand(std::string command) {
    size_t spacePos = command.find(' ');
    std::string cmd = (spacePos == std::string::npos) ? command : command.substr(0, spacePos);
    std::string arg = (spacePos == std::string::npos) ? "" : command.substr(spacePos + 1);

    auto it = commandMap.find(cmd);
    if (it != commandMap.end()) {
        it->second(arg);
    } else {
        std::cout << "Invalid command" << std::endl;
    }
}

void Game::displayHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  help  - Display this help message" << std::endl;
    std::cout << "  look  - Look around" << std::endl;
    std::cout << "  go [direction] - Move in a direction (north, south, east, west)" << std::endl;
    std::cout << "  pickup [item] - Pick up an item" << std::endl;
    std::cout << "  backpack - Check your inventory" << std::endl;
    std::cout << "  drop [item] - Drop an item" << std::endl;
    std::cout << "  talk [npc] - Talk to an NPC" << std::endl;
    std::cout << "  give [npc] [item] - Give an item to an NPC" << std::endl;
    std::cout << "  quit  - Quit the game" << std::endl;
}

void Game::go(std::string target) {
    auto locations = curLocation->get_locations();
    
    // Convert the input direction to lowercase
    std::transform(target.begin(), target.end(), target.begin(), ::tolower);

    // Find the direction in the map by comparing lowercase versions
    auto it = std::find_if(locations.begin(), locations.end(), [&target](const auto& pair) {
        std::string direction = pair.first;
        std::transform(direction.begin(), direction.end(), direction.begin(), ::tolower);
        return direction == target;
    });

    if (it == locations.end()) {
        std::cout << "You cannot go that way." << std::endl;
        return; 
    }

    std::cout << "You moved." << std::endl;
    curLocation = &it->second.get(); // Move to the new location
}

void Game::take(std::string itemName) {
    auto& items = curLocation->getItems();

    // Item names are stored Capitalized. Commands are input in lowercase.
    // This searches for items in the current locations items and compares the lowercase version of the item name
    auto it = std::find_if(items.begin(), items.end(), [&itemName](const std::reference_wrapper<Item>& item) {
        std::string itemNameInList = item.get().getName();
        // Convert itemNameInList to lowercase for case-insensitive comparison
        std::transform(itemNameInList.begin(), itemNameInList.end(), itemNameInList.begin(), ::tolower);
        return itemNameInList == itemName;
    });

    // If item in location, add to player inventory, remove from location, and update player carry weight
    if (it != items.end()) {
        playerInventory.push_back(it->get());
        playerCarryWeight += it->get().getWeight();
        curLocation->removeItem(it->get().getName()); 
        std::cout << "You picked up " << itemName << "." << std::endl;
    } else {
        std::cout << "Item not found." << std::endl;
    }
}

void Game::drop(std::string itemName) {
    auto it = std::find_if(playerInventory.begin(), playerInventory.end(), [&itemName](const Item& item) {
        std::string itemNameInList = item.getName();
        std::transform(itemNameInList.begin(), itemNameInList.end(), itemNameInList.begin(), ::tolower);
        return itemNameInList == itemName;
    });

    if (it != playerInventory.end()) {
        curLocation->addItem(*it);
        playerCarryWeight -= it->getWeight();
        playerInventory.erase(it);
        std::cout << "You dropped " << itemName << "." << std::endl;
    } else {
        std::cout << "Item not found." << std::endl;
    }
}

void Game::show_items() {
    std::cout << "Your inventory:" << std::endl;
    for (const auto& item : playerInventory) {
        std::cout << "  " << item << std::endl;
    }
    std::cout << "Total carry weight: " << playerCarryWeight << "/30 lbs" << std::endl;
}

void Game::talk(const std::string& npcName) {
    const auto& npcs = curLocation->get_npcs();
    auto it = std::find_if(npcs.begin(), npcs.end(),
        [&npcName](const std::reference_wrapper<NPC>& npc) {
            std::string npcNameInList = npc.get().get_name();
            std::transform(npcNameInList.begin(), npcNameInList.end(), 
                          npcNameInList.begin(), ::tolower);
            return npcNameInList == npcName;
        });
    if (it != npcs.end()) {
        std::cout << it->get().get_message() << std::endl;
    } else {
        std::cout << "NPC not found." << std::endl;
    }
}

void Game::give(const std::string& npcName, const std::string& itemName) {
    std::cout << "[You give the " << itemName << " to the " << npcName << ".]" << std::endl;
    
    // Find the NPC in the current location
    const auto& npcs = curLocation->get_npcs();
    auto npcIt = std::find_if(npcs.begin(), npcs.end(),
        [&npcName](const std::reference_wrapper<NPC>& npc) {
            std::string npcNameInList = npc.get().get_name();
            std::transform(npcNameInList.begin(), npcNameInList.end(), 
                          npcNameInList.begin(), ::tolower);
            return npcNameInList == npcName;
    });
    
    if (npcIt == npcs.end()) {
        std::cout << "NPC not found." << std::endl;
        return;
    }
    
    // Find the item in the player's inventory
    auto itemIt = std::find_if(playerInventory.begin(), playerInventory.end(), 
                              [&itemName](const Item& item) {
        std::string itemNameInList = item.getName();
        std::transform(itemNameInList.begin(), itemNameInList.end(), 
                      itemNameInList.begin(), ::tolower);
        return itemNameInList == itemName;
    });
    
    if (itemIt == playerInventory.end()) {
        std::cout << "Item not found in inventory." << std::endl;
        return;
    }
    
    // Remove the item from the player's inventory
    playerCarryWeight -= itemIt->getWeight();
    playerInventory.erase(itemIt);
    
    if (npcName == "troll" && itemName == "gold coin") {
        std::cout << "I didn't think you'd actually find a gold coin. Fair enough, go along." << std::endl;
        
        // I spent way too long on this, so I went to Copilot. instead of using curLocation->get_locations(), it just goes through locationPtrs.
        // Then it just looks for a library pointer. It gets dereferenced twice because we made a pointer to the pointer. 
        // It's a bit messy but it works well. 
        auto libIt = std::find_if(locationPtrs.begin(), locationPtrs.end(),
            [](Location* loc) { return loc->getName() == "Library"; });
            
        if (libIt != locationPtrs.end()) {
            // Add North connection to current location pointing to Library 
            curLocation->add_location("North", **libIt);
            std::cout << "[You can now go north to the library.]" << std::endl;
        }
    }
    else if (npcName == "wishgranter") {
        if (itemIt->getCalories() == 0) {
            std::cout << ">> Why'd you give me that? Maybe I should've just done the blood sacrifice. Hey look what I can do! <<" << std::endl;
            teleport();
            std::cout << "[You were teleported far away. You are now in " << curLocation->getName() << ".]" << std::endl;
            return;
        }
        else {
            wishgranterCalories += itemIt->getCalories();
            std::cout << "The unearthly monolith glows as you give it the " << itemName << "." << std::endl;
            std::cout << "The Wishgranter's calories: " << wishgranterCalories << "/500" << std::endl;
            
            if (wishgranterCalories >= 500) {
                youWin();
                return;
            }
        }
    }
    else if (npcName == "levi" && itemName == "levi's journal") {
        std::cout << "Oh, it was right there? Well, a deal's a deal" << std::endl;
        std::cout << "[You were given a can of beans.]" << std::endl;
        Item beans = Item("Beans", "A can of beans.", 50, 1.0);
        playerCarryWeight += beans.getWeight();
        playerInventory.push_back(beans);
    }
    else if (npcName == "driver" && itemName == "bus pass") {
        std::cout << "That's gotta be the last bus pass in GVSU. I'll take you to Standale." << std::endl;
        std::cout << "[You now have access to Standale locations. Head north to check them out.]" << std::endl;
        
        
        // Find Mackinac and Meijer locations
        auto mackinacIt = std::find_if(locationPtrs.begin(), locationPtrs.end(),
            [](Location* loc) { return loc->getName() == "Mackinac"; });
        auto meijerIt = std::find_if(locationPtrs.begin(), locationPtrs.end(),
            [](Location* loc) { return loc->getName() == "Meijer"; });
            
        if (mackinacIt != locationPtrs.end() && meijerIt != locationPtrs.end()) {
            // Add East connection from Mackinac to Meijer
            (**mackinacIt).add_location("East", **meijerIt);
            curLocation->add_location("North", **meijerIt);

        }
        
    }
    else {
        std::cout << "Why would I want this? I'm not giving it back but I'm not grateful." << std::endl;
    }
}

void Game::teleport() {
    // Create a random number generator (thanks copilot)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, locationPtrs.size() - 1);

    // Select a random location
    int randomIndex = dis(gen);

    // This is only used by the wishgranter, so we want it to be any location besides Housing
    while (locationPtrs[randomIndex]->getName() == "On Campus Housing") {
        randomIndex = dis(gen);
    }
    curLocation = locationPtrs[randomIndex];
}


void Game::createWorld() {
    // initalize player carry weight and wishgranter calories
    playerCarryWeight = 0.0;
    wishgranterCalories = 0.0;

    // Create Location objects on the heap
    Location* kirkoff = new Location("Kirkoff", "The central bus station for GVSU.");
    Location* stadium = new Location("Stadium", "The GVSU soccer field.");
    Location* housing = new Location("On Campus Housing", "GVSU's home to Lakers.");
    Location* library = new Location("Library", "The library located on the Allendale campus.");
    Location* levisHouse = new Location("Levi's House", "A GVSU student's house.");
    Location* mackinac = new Location("Mackinac", "Home of GVSU computing.");
    Location* bridge = new Location("Bridge", "A bridge over the valley. It's a long way down.");
    Location* meijer = new Location("Meijer", "A ransacked grocery store.");

    locationPtrs.push_back(kirkoff);
    locationPtrs.push_back(stadium);
    locationPtrs.push_back(housing);
    locationPtrs.push_back(library);
    locationPtrs.push_back(levisHouse);
    locationPtrs.push_back(mackinac);
    locationPtrs.push_back(bridge);
    locationPtrs.push_back(meijer);

    // Define movement directions
    kirkoff->add_location("West", *library);
    kirkoff->add_location("South", *stadium);
    // kirkoff->add_location("North", *meijer);

    stadium->add_location("South", *housing);
    stadium->add_location("North", *kirkoff);
    stadium->add_location("West", *bridge);

    housing->add_location("North", *stadium);

    library->add_location("East", *kirkoff);
    library->add_location("North", *mackinac);
    library->add_location("South", *bridge);

    // mackinac->add_location("East", *meijer);
    mackinac->add_location("South", *library);

    levisHouse->add_location("West", *meijer);

    meijer->add_location("West", *mackinac);
    meijer->add_location("South", *kirkoff);
    meijer->add_location("East", *levisHouse);

    bridge->add_location("East", *stadium);

    // Set starting location
    curLocation = housing; // Set curLocation to point to housing

    // Create item objects
    Item* book = new Item("Book", "A book for reading.", 0, 0.5);
    Item* levisJournal = new Item("Levi's Journal", "A journal with notes about C++.", 0, 1.0);
    Item* busPass = new Item("Bus Pass", "A bus pass for the Laker Line.", 0, 0.1);
    Item* cake = new Item("Chocolate Cake", "Must've been a birthday party.", 150, 5.0);
    Item* nuggies = new Item("Nuggies", "6 piece nugget.", 50, 1.0);
    Item* granolaBar = new Item("Granola Bar", "A light snack.", 25, 0.5);
    Item* bread = new Item("Bread", "A loaf of bread.", 50, 1.0);
    Item* fish = new Item("Fish", "A live fish with legs.", 100, 3.0);
    Item* helmet = new Item("Helmet", "A helmet for protection.", 0, 5.0);
    
    Item* knife = new Item("Knife", "A knife for protection.", 0, 4.0);
    Item* goldCoin = new Item("Gold Coin", "A conveniently placed coin. It might be worthwhile taking this with.", 0, 0.1);
    Item* sushi = new Item("Sushi", "Even in an apocalypse, Meijer still has killer deals on sushi.", 50, 2.0);

    Item* debugCookie = new Item("Debug Cookie", "A cookie", 500, 0.1);
    housing->addItem(*debugCookie);

    // Add items to locations
    stadium->addItem(*helmet);
    levisHouse->addItem(*levisJournal);
    library->addItem(*book);
    kirkoff->addItem(*busPass);
    kirkoff->addItem(*cake);
    mackinac->addItem(*nuggies);
    levisHouse->addItem(*granolaBar);
    library->addItem(*bread);
    stadium->addItem(*fish);
    stadium->addItem(*goldCoin);
    meijer->addItem(*sushi);
    housing->addItem(*knife);


    // Create NPC objects
    NPC* monolith = new NPC("Wishgranter", "A monolithic rock that speaks to you.");
    NPC* levi = new NPC("Levi", "A GVSU student who loves C++.");
    NPC* busDriver = new NPC("Driver", "The driver of the Laker Line.");
    NPC* librarian = new NPC("Librarian", "The librarian at the GVSU library.");
    NPC* troll = new NPC("Troll", "A troll guarding the bridge.");

    monolith->set_messages({"Approach, young one. I bring you an offer.", \
                            "I know you miss all the homework and stress that came with classes before the world ended.", \
                            "I can do anything imagineable, but I'm a very hungry deity. Bring me some food and I'll grant you one wish.", \
                            "Usually I'd ask for a blood sacrifice but I'm feeling generous today."});

    
    levi->set_messages({"Hey, I'm Levi.", "Ever since the world ended I've been so bored. I can't even code C++ anymore!", \
                         "If you find my journal around here, I'll trade you some beans for it!"});

    
    troll->set_messages({"This is my bridge. I'll only grant you passage if you give me a Gold Coin!", \
                        "I know this is stereotypical, but I love this bridge. Easily top 5 bridges of all time."});
                        
    librarian->set_messages({"Welcome to the library. I'm the librarian.", "I'm sorry, but I can't let you check out any books. They're all overdue."});

    busDriver->set_messages({"Welcome to the Laker Line. I'm the bus driver.", "If you bring me a bus pass, I'll give you access to Standale.", \
                            "I don't care that the world ended, a bus pass is a bus pass.", "I've been siphoning gas all day, got me seeing things..."});

    


    // Add NPCs to locations
    library->add_npc(*librarian);
    kirkoff->add_npc(*busDriver);
    levisHouse->add_npc(*levi);
    housing->add_npc(*monolith);
    bridge->add_npc(*troll);
}

void Game::youWin() {
    std::cout << ">> That was a weird meal, but it was good enough. What's your wish mortal?\n" << std::endl;
    std::cout << "You wish for the world to be normal again.\n" << std::endl;
    std::cout << ">> That's such a lame wish. You could've wished for a utopia or something but you just wanted the status quo? Whatever. <<\n" << std::endl;
    std::cout << "The world is back to normal. You're late for class." << std::endl;
    std::cout << ">> YOU WIN! <<" << std::endl;
    isRunning = false;
    exit();
}

void Game::exit() {
    for (auto location : locationPtrs) {
        delete location;
    }
}

