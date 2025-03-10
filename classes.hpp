#ifndef     __HPP__CLASSES__
#define     __HPP__CLASSES__

// Created By: Levi Klamer, Mason Passinault, Blaze Lauer

#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <functional>
#include <algorithm> // Include this header for std::remove_if


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

        std::string getName() const {
          return name;
        }

        float getCalories() const {
          return calories;
        }

        float getWeight() const {
          return weight;
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
            this->messageNum = 0;
        }

        std::string get_name() {
          return this->name;
        }

        std::string get_description() {
          return this->description;
        }

        void set_name(std::string name) {
          this->name = name;
        }

        void set_description(std::string description) {
          this->description = description;
        }

        // getter for messages that returns the current message/changes message number
        void set_messages(const std::vector<std::string>& messages) {
          this->messages = messages;
          this->messageNum = 0;
        }

        std::string get_message() {
          if (messages.empty()) {
              return "";
          }
          std::string message = messages[messageNum];
          messageNum = (messageNum + 1) % messages.size();
          return message;
        }

    private:
        std::string name;
        std::string description;
        std::vector<std::string> messages;
        size_t messageNum;

    friend std::ostream& operator<<(std::ostream& os, const NPC& npc) {
        os << npc.name;
        return os;
    }
};


class Location {
    public:
        Location(){}
        Location(std::string name, std::string description) {
            if (name.empty()) {
                throw std::invalid_argument("Name cannot be blank.");
            }
            if (description.empty()) {
                throw std::invalid_argument("Description cannot be blank.");
            }

            this->name = name;
            this->description = description;
            this->visited = false;
        }

        // methods that get
        std::map<std::string, std::reference_wrapper<Location>> get_locations() {
          return this->neighbors;
        }

        std::vector<std::reference_wrapper<NPC>> get_npcs() {
          return this->npcs;
        }

        std::vector<std::reference_wrapper<Item>> get_items() {
          return this->items;
        }

        std::string get_description() {
          return this->description;
        }

        bool get_visited() {
          return this->visited;
        }

        // methods that add to lists and set
        void add_location(const std::string& direction, Location& location) {
          if (direction.empty()) {
              throw std::invalid_argument("Direction cannot be blank.");
          }

          if (this->neighbors.find(direction) != this->neighbors.end()) {
              throw std::invalid_argument("Direction is already being used");
          }

          this->neighbors.emplace(direction, std::ref(location));
        }

        void add_npc(NPC& npc) {
          this->npcs.push_back(npc);
        }

        void addItem(Item& item) {
          this->items.push_back(item);
        }

        void set_visited() {
          this->visited = true;
        }

        const std::string& getName() const {
          return this->name;
        }

        std::vector<std::reference_wrapper<Item>>& getItems() {
          return this->items;
        }
  
        void removeItem(const std::string& itemName) {
          auto initialSize = items.size();
          items.erase(std::remove_if(items.begin(), items.end(), [&itemName](const std::reference_wrapper<Item>& item) {
              return item.get().getName() == itemName;
          }), items.end());
          auto finalSize = items.size();
        }

    
    private:
        std::string name;
        std::string description;
        bool visited;
        std::map<std::string, std::reference_wrapper<Location>> neighbors;
        std::vector<std::reference_wrapper<NPC>> npcs;
        std::vector<std::reference_wrapper<Item>> items;

    friend std::ostream& operator<<(std::ostream& os, const Location& location) {
        os << "Current Location: " << location.name << " - " << location.description << "\n";
        os << "You see the following NPCs:\n";
        // list NPCs
        for(auto& npc : location.npcs) {
          os << "- " << npc.get() << "\n";
        }

        os << "You see the following Items:\n";
        // list Items
        for (const auto& item : location.items) {
          os << "- " << item.get() << "\n";
        }

        os << "You can go in the following Directions:\n";
        // list locations/directions that are connected to this Location
        for(auto& pair : location.neighbors) {
          os << "- " << pair.first << " - " << pair.second.get().getName() << " - " << pair.second.get().get_description() << "\n";
        }
        
        return os;
    }
};

#endif
