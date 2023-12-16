#include "../include/Game.h"
#include "../include/Error.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // std::shuffle
#include <random>    // std::default_random_engine
#include <chrono>    // std::chrono::system_clock
#include <tuple>
#include <iostream>


System::Game::Game(){
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

std::vector<int> System::Game::getRandStoryIds(int num) {
    std::vector<int> ids;

    // 創建一個有所有可能索引的向量
    for (int i = 0; i < this->stories.size(); ++i) {
        ids.push_back(i);
    }

    // 使用當前時間作為隨機數生成器的種子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    // 打亂向量
    std::shuffle(ids.begin(), ids.end(), std::default_random_engine(seed));

    // 只保留所需數量的元素
    if (num < ids.size()) {
        ids.resize(num);
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

void System::Game::setOptions(int id, const std::vector< System::Option > &choices) {
    System::Story* story_ptr = this->getStoryPtrById(id);
    if (story_ptr == nullptr) {
        std::cout<<"story_ptr is nullptr"<<std::endl;
        return;
    }
    story_ptr->choices = choices;
}

void System::Game::printOptions(int id) {
    System::Story* story_ptr = this->getStoryPtrById(id);
    if (story_ptr == nullptr) {
        return;
    }
    std::cout << "現在你在" << story_ptr->place << "，" << story_ptr->content << std::endl;
    for (auto& choice : story_ptr->choices) {
        std::cout << choice.id << ": " << choice.text << std::endl;
    }
}

bool System::Game::setUserChoice(int story_id, const std::string &user_choice_id) {
    System::Story* story_ptr = this->getStoryPtrById(story_id);
    if (story_ptr == nullptr) {
        return false;
    }
    for (auto& choice : story_ptr->choices) {
        if (choice.id == user_choice_id) {
            story_ptr->user_choice = choice.text;
            story_ptr->is_answered = true;
            return true;
        }
    }
    return true;
}

OpenAI::Message System::Game::generateStoryPrompt(int story_index) {
    std::string prompt = "";
    // get previous story
    if(this->current_count == 0){
        prompt = "我在" + this->stories[story_index].place + "，我" + this->stories[story_index].content;
    }else{
        int previous_id = this->story_ids[story_index - 1];
        prompt = "我在「" + this->stories[story_index].place + "，" + this->stories[story_index].content + "」";
    }

    return OpenAI::Message({"user", prompt});
}