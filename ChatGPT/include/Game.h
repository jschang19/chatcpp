#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "ChatGPT.h"

namespace System {
struct Option {
  std::string id;
  std::string text;
  Option(std::string id, std::string text)
      : id(id), text(text){};  // constructor
};

class Story {
 public:
  int id;
  std::string place;
  std::string content;
  std::string user_choice;
  bool is_answered = false;
  std::vector<System::Option> choices;
  Story(int id, std::string place, std::string content)
      : id(id), place(place), content(content) {}
  ~Story();  // destructor
 private:
};

class Game {
  public:
    //setter

    void set_count(int new_count);
    void set_story_id(std::vector<int> new_ids);
    void add_current_count(int adding_num);


    void addPrompt(OpenAI::ChatGPT& chatGpt, const OpenAI::Message& prompt,bool isLast);
    OpenAI::ChatCompletion sendToChatGPT(OpenAI::ChatGPT& chatGpt, bool isEnding);
    std::vector<int> picked_story_ids;
    void parseGPTResponse(OpenAI::ChatCompletion& chatCompletion, int story_id);
    void parseEndingResponse(OpenAI::ChatCompletion& chatCompletion);
    Game();
    ~Game();  // destructor
    std::vector<System::Story> stories;
    std::vector<int> getRandStoryIds(int num);
    System::Story* getStoryPtrById(int id);
    OpenAI::Message generateStoryPrompt(int id);
    OpenAI::Message generateEndingPrompt();
    void setOptions(int id, const std::vector<System::Option>& choices);
    void printOptions(int id);
    void print(const std::string& str, const std::string& color = "w",bool bold = false);
    void printWelcome();
    void checkStatus(const std::string& key);
    bool verifyOpenAiKey(const std::string& key);
    bool checkConnection();
    bool setUserChoice(int story_id, std::string user_choice_id);

  private:
    std::vector<System::Story> readTextFile(const std::string& filename);
    int count;
    int current_count = 0;
    std::vector<int> story_ids;
};
}  // namespace System