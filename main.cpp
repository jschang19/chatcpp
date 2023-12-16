#include <iostream>
#include <string>
#include <vector>
#include "ChatGPT/include/ChatGPT.h"
#include "ChatGPT/include/Error.h"
#include "ChatGPT/include/Game.h"
// this is the main function
int main(int args,char** argv){
    //API token as argument
    // check if argument is passed
    if(args<2){
        std::cout<<"Please provide the API token as argument"<<std::endl;
        return 0;
    }
    System::Game game;
    OpenAI::ChatGPT chatGpt{argv[1]};
    // get random story ids
    std::vector<int> story_ids = game.getRandStoryIds(3);
    // get story pointers
    std::vector<System::Story*> story_ptrs;

    // print out the stories
    for (int i = 0; i < story_ids.size(); i++) {
        story_ptrs.push_back(game.getStoryPtrById(story_ids[i]));
        std::cout << story_ptrs[i]->content << std::endl;
    }
    try {
        // get user input
        std::string userInput;
        std::cout<<"Enter your message: ";
        std::getline(std::cin,userInput);
        auto response = chatGpt.askChatGPT("user");
        //Iterate all answers
        for(const auto& choice:response.choices){
            std::cout<<choice.message.content;
        }
    }catch(OpenAI::Error& e){
        //JSON error returned by the server
        std::cout<<e.what();
    }
}