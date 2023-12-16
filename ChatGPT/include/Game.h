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
        void print(const std::string& str, const std::string& color = "w", bool bold=false){
          std::string color_code;
          switch (color[0])
            {
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
          if(bold){
            std::cout << "\033[1;" << color_code << "m" << str << "\033[0m\n";
            }
            else{
            std::cout << "\033[" << color_code << "m" << str << "\033[0m\n";
            }
        };
        void printWelcome();
        void checkStatus(const std::string& key);
        bool verifyOpenAiKey(const std::string& key);
        bool checkConnection();
        bool setUserChoice(int story_id, std::string user_choice_id);
    private:
        std::vector< System::Story > readTextFile(const std::string& filename);
    };
}