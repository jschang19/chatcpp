#include <iostream>
#include <string>
#include <vector>
#include "ChatGPT/include/Error.h"
#include "ChatGPT/include/Game.h"
// this is the main function

const int STORY_NUM=5;

int main(int args,char** argv){
    //API token as argument
    // check if argument is passed
    if(args<2){
        std::cout<<"Please provide the API token as argument"<<std::endl;
        return 0;
    }
    System::Game game;
    game.count=STORY_NUM;
    OpenAI::ChatGPT chatGpt{argv[1]};
    // get random story ids
    std::vector<int> story_ids = game.getRandStoryIds(STORY_NUM);
    game.story_ids = story_ids;
    // get story pointers
    std::vector<System::Story*> story_ptrs;


    try {
        for (int i=0; i<STORY_NUM; i++){
            int story_id = story_ids[i];
            OpenAI::Message prompt = game.generateStoryPrompt(i);
            game.addPrompt(chatGpt, story_id);
            auto chatCompletion = game.sendToChatGPT(chatGpt);
            game.parseGPTResponse(chatCompletion, story_id);
            game.printOptions(story_id);

            std::string userInput;
            std::cout<<"輸入你的選擇:";
            std::getline(std::cin,userInput);

            game.setUserChoice(story_id, userInput);
            game.current_count += 1;
        }
    }catch(OpenAI::Error& e){
        //JSON error returned by the server
        std::cout<<e.what();
    }
}