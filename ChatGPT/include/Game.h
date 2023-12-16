#include <vector>
#include <string>
#include "ChatGPT.h"
#include <nlohmann/json.hpp>

namespace System {
    struct Option{
        std::string id;
        std::string text;
        Option(std::string id, std::string text) : id(id), text(text) {}; // constructor
    };

    class Story {
    public:
        int id;
        std::string place;
        std::string content;
        std::string user_choice;
        bool is_answered = false;
        std::vector< System::Option > choices;
        Story(int id, std::string place, std::string content)
        : id(id), place(place), content(content) {}
        ~Story(); // destructor
    private:
    };
    

    class Game {
    public:
        int count;
        int current_count = 0;
        std::vector<int> story_ids;
        void addPrompt(OpenAI::ChatGPT& chatGpt, const int story_id){
            OpenAI::Message prompt = this->generateStoryPrompt(story_id);
            chatGpt.Add_prompt(prompt);
        };
        OpenAI::ChatCompletion sendToChatGPT(OpenAI::ChatGPT& chatGpt){
            auto response = chatGpt.askChatGPT("user");
            return response;
        };
        std::vector<int> picked_story_ids;
        void parseGPTResponse(OpenAI::ChatCompletion& chatCompletion, int story_id){
            System::Story* story_ptr = this->getStoryPtrById(story_id);
            nlohmann::json j2;
            try {
                j2 = nlohmann::json::parse(chatCompletion.choices[0].message.content);
                std::vector< System::Option > choices;
                for (auto& choice : j2["options"]) {
                    choices.push_back(System::Option(choice["id"], choice["text"]));
                }
                this->setOptions(story_id, choices);
            }catch(std::exception& e){
                std::cerr<<"Game.h parsing Error: "+chatCompletion.choices[0].message.content;
            }
        };
        Game();
        ~Game(); // destructor
        std::vector< System::Story > stories;
        std::vector< int > getRandStoryIds(int num);
        System::Story* getStoryPtrById(int id);
        OpenAI::Message generateStoryPrompt(int id);
        void setOptions(int id, const std::vector< System::Option > &choices);
        void printOptions(int id);
        bool setUserChoice(int story_id, const std::string& user_choice_id);
    private:
        std::vector< System::Story > readTextFile(const std::string& filename);
    };
}