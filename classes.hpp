#ifndef     __HPP__CLASSES__
#define     __HPP__CLASSES__

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>

class Item {
    public:
        Item(std::string name, std::string description, int calories, float weight) {
            if (name.empty()) {
                throw std::invalid_argument("Name cannot be blank.");
            }
            if (description.empty()) {
                throw std::invalid_argument("Description cannot be blank.");
            }
            if (calories < 0 || calories > 1000) {
                throw std::invalid_argument("Calories must be between 0 and 1000.");
            }
            if (weight < 0.0f || weight > 500.0f) {
                throw std::invalid_argument("Weight must be between 0 and 500.");
            }
            
            this->name = name;
            this->description = description;
            this->calories = calories;
            this->weight = weight;
        }

    private:
        std::string name;
        std::string description;
        int calories;
        float weight;

    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        os << item.name << " (" << item.calories << " calories) - " << item.weight << " lb - " << item.description;
        return os;
    }
};


class NPC {
    public:
        NPC(std::string name, std::string description) {
            if (name.empty()) {
                throw std::invalid_argument("Name cannot be blank.");
            }
            if (description.empty()) {
                throw std::invalid_argument("Description cannot be blank.");
            }

            this->name = name;
            this->description = description;
        }

        // getters for name and description

        // getter for messages that returns the current message/changes message number

    private:
        std::string name;
        std::string description;
        std::vector<std::string> messages;
        int messageNum;

    friend std::ostream& operator<<(std::ostream& os, const NPC& npc) {
        os << npc.name;
        return os;
    }
};


class Location {
    public:
        Location(std::string name, std::string description) {
            if (name.empty()) {
                throw std::invalid_argument("Name cannot be blank.");
            }
            if (description.empty()) {
                throw std::invalid_argument("Description cannot be blank.");
            }

            this->name = name;
            this->description = description;
        }

        // get_locations()

        // add_location(string direction, Location location)

        // add_npc(NPC npc)

        // add_item(Item item)

        // set_visited()

        // get_visited()
    
    private:
        std::string name;
        std::string description;
        bool visited;
        std::map<std::string, Location> neighbors;
        std::vector<NPC> npcs;
        std::vector<Item> items;

    friend std::ostream& operator<<(std::ostream& os, const Location& location) {
        os << location.name << " - " << location.description << "\n";
        os << "You see the following NPCs:\n";
        // list NPCs
        os << "You see the following Items:\n";
        // list Items
        os << "You can go in the following Directions:\n";
        // list locations/directions that are connected to this Location
        return os;
    }
};

#endif