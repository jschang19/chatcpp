#include "../include/Game.h"
#include "../include/Error.h"
#include <cpr/cpr.h> // verify api key
#include <curl/curl.h>
#include <vector>
#include <stdlib.h> 
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // std::shuffle
#include <random>    // std::default_random_engine
#include <chrono>    // std::chrono::system_clock
#include <string>
#include <tuple>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib> // for connection check

void System::Game::set_count(int new_count){
    this->count=new_count;
}
void System::Game::set_story_id(std::vector<int> new_ids){
    this->story_ids=new_ids;
}
void System::Game::add_current_count(int adding_num){
    this->current_count+=adding_num;
}


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

    for (int i = 0; i < this->stories.size(); ++i) {
        ids.push_back(i);
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(ids.begin(), ids.end(), std::default_random_engine(seed));

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
    this->print("\u2753 現在你在" + story_ptr->place + "，" + story_ptr->content, "w", false);
    std::cout << std::endl;
    for (auto& choice : story_ptr->choices) {
        // std::cout << choice.id << ": " << choice.text << std::endl;
        this->print("["+choice.id + "] " + choice.text, "w", false);
    }
}

bool System::Game::setUserChoice(int story_id, std::string user_choice_id) {
    System::Story* story_ptr = this->getStoryPtrById(story_id);
    if (story_ptr == nullptr) {
        std::cout<<"story_ptr is nullptr"<<std::endl;
        return false;
    }
    std::transform(user_choice_id.begin(), user_choice_id.end(), user_choice_id.begin(), ::tolower);
    for (auto& choice : story_ptr->choices) {
        if (choice.id == user_choice_id) {
            story_ptr->user_choice = choice.text;
            story_ptr->is_answered = true;
            return true;
        }
    }
    return true;
}

void System::Game::printWelcome(){
    this->print("\U0001F4D6 這是一個結合 ChatGPT 的文字冒險遊戲。","w");
    this->print("\U0001F468\u200D\U0001F393 你將扮演一位大學生，並在這個遊戲中做出選擇。請輸入你想要的選項英文代號","w");
    std::cout<<std::endl;
    this->print("注意事項","w");
    this->print("\u2139 本遊戲的選項由 ChatGPT 自動生成，如有雷同純屬巧合。","r");
    this->print("\U0001F6A9 ChatGPT 可能會產生錯誤或不符情境的選項。","r");
    std::cout<<std::endl;
    this->print("遊戲將在 3 秒後開始","w");
    std::cout<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void System::Game::checkStatus(const std::string& api_key){
    
    this->print("檢查網路連線...","l");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    if(!this->checkConnection()){
        this->print("\u2717 沒有網路連線，請檢查網路連線後再重新啟動遊戲","r", true);
        exit(0);
    }else{
        this->print("\u2714 網路連線正常","g");
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    this->print("檢查 API key...","l");
    if(!this->verifyOpenAiKey(api_key)){
        this->print("\u2717 API key 驗證失敗，請檢查 API key 是否正確","r", true);
        exit(0);
    }else{
        this->print("\u2714 API key 驗證成功","g");
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    this->print("\u2714 遊戲已啟動！","g");
}

bool System::Game::verifyOpenAiKey(const std::string& apiKey) {
    // Using the models endpoint for a lighter request
    std::string endpoint = "https://api.openai.com/v1/models";

    // Making a GET request to the models endpoint
    cpr::Response response = cpr::Get(cpr::Url{endpoint},
                                      cpr::Header{{"Authorization", "Bearer " + apiKey}});

    // Check response status code
    if (response.status_code == 200) {
        // Status code 200 indicates a successful response
        return true;
    } else {
        // Any other status code indicates a problem
        //std::cerr << "\u2717 Failed to verify API key. Response code: " << response.status_code << std::endl;
        this->print("\u2717 Failed to verify API key. Response code: " + std::to_string(response.status_code), "r");
        if (response.status_code == 401) {
            this->print("請檢查 API key 是否正確","r", true);
        }
        return false;
    }
}

bool System::Game::checkConnection() {
    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com");
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1); // HEAD request
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        return (res == CURLE_OK);
    }
    return false;
}

OpenAI::Message System::Game::generateStoryPrompt(int story_index) {
    std::string prompt = "";
    // get previous story
    if(this->current_count == 0){
        prompt = "我在" + this->stories[story_index].place + this->stories[story_index].content + "。";
    }else{
        int previous_id = this->story_ids[story_index - 1];
        prompt +=( "我剛剛在" + this->stories[previous_id].place + this->stories[previous_id].content + "，我選擇「" + this->stories[previous_id].user_choice + "。」");
        prompt += "現在我在" + this->stories[story_index].place + "，" + this->stories[story_index].content + "。";
    }

    return OpenAI::Message({"user", prompt});
}

OpenAI::Message System::Game::generateEndingPrompt(){
    std::string final_prompt = "";
    final_prompt = "我在「" + this->stories[this->current_count].place + "，" + this->stories[this->current_count].content + "」";
    
    return OpenAI::Message({"user", final_prompt});
}

 void System :: Game :: addPrompt(OpenAI::ChatGPT& chatGpt, const OpenAI::Message& prompt,bool isLast) {
    if (isLast) {
      OpenAI::Message prompt = this->generateEndingPrompt();
        chatGpt.addPrompt(prompt);
    } else {
      chatGpt.addPrompt(prompt);
    }

  };

OpenAI::ChatCompletion System :: Game ::sendToChatGPT(OpenAI::ChatGPT& chatGpt, bool isEnding) {
    this->print("ChatGPT 正在思考中...", "l");
    std::cout << std::endl;
    auto response = chatGpt.askChatGPT("user", isEnding);
    return response;
}

void System::Game::parseGPTResponse(OpenAI::ChatCompletion& chatCompletion, int story_id) {
    System::Story* story_ptr = this->getStoryPtrById(story_id);
    nlohmann::json j2;
    try {
      j2 = nlohmann::json::parse(chatCompletion.choices[0].message.content);
      std::vector<System::Option> choices;
      for (auto& choice : j2["options"]) {
        choices.push_back(System::Option(choice["id"], choice["text"]));
      }
      this->setOptions(story_id, choices);
    } catch (std::exception& e) {
      std::cerr << "Game.h parsing Error: " +
                       chatCompletion.choices[0].message.content;
    }
}

void System::Game::parseEndingResponse(OpenAI::ChatCompletion& chatCompletion) {
    try {
     for(const auto& choice:chatCompletion.choices){
            std::cout<<choice.message.content;
        }
    } catch (std::exception& e) {
      std::cerr << "Game.h ending parsing Error: " +
                       chatCompletion.choices[0].message.content;
    }
}

void System::Game::print(const std::string& str, const std::string& color ,bool bold ) {
    std::string color_code;
    switch (color[0]) {
      case 'r':
        color_code = "31";
        break;
      case 'g':
        color_code = "32";
        break;
      case 'y':
        color_code = "33";
        break;
      case 'b':
        color_code = "34";
        break;
      case 'p':
        color_code = "35";
        break;
      case 'c':
        color_code = "36";
        break;
      case 'w':
        color_code = "37";
        break;
      case 'l':
        color_code = "90";
        break;
      default:
        color_code = "37";
        break;
    }
    if (bold) {
      std::cout << "\033[1;" << color_code << "m" << str << "\033[0m\n";
    } else {
      std::cout << "\033[" << color_code << "m" << str << "\033[0m\n";
    }
  };