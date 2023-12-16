#include "../include/Game.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <iostream>

System::Game::Game() {
    // get data from file
    try {
        this->stories = readTextFile("ChatGPT/data/stories.txt");
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
}

System::Game::~Game() {
    this->stories.clear();
}

System::Story::~Story() {
    this->choices.clear();
}

std::vector< int > System::Game::getRandStoryIds(int num) {
    std::vector< int > ids;
    for (int i = 0; i < num; i++) {
        ids.push_back(rand() % this->stories.size());
    }
    return ids;
}

System::Story* System::Game::getStoryPtrById(int id) {
    for (int i = 0; i < this->stories.size(); i++) {
        if (this->stories[i].id == id) {
            return &this->stories[i];
        }
    }
    return nullptr;
}

std::vector<System::Story> System::Game::readTextFile(const std::string& filename) {
    std::vector<System::Story> data;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int id;
        std::string place, content;
        
        if (!(iss >> id >> place >> content)) {
            // Handle error or skip the malformed line
            continue;
        }

        data.push_back(System::Story(id, place, content));
    }

    return data;
}