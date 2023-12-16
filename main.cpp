#include <iostream>
#include <string>
#include <vector>
#include "ChatGPT/include/ChatGPT.h"
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
    OpenAI::ChatGPT chatGpt{argv[1]};
    // get random story ids
    std::vector<int> story_ids = game.getRandStoryIds(STORY_NUM);
    // get story pointers
    std::vector<System::Story*> story_ptrs;


    try {
        for (int i=0; i<STORY_NUM; i++){
            // get user input
            std::string userInput;
            std::cout<<"Enter your message: ";
            std::getline(std::cin,userInput);
            chatGpt.Add_prompt("user","answer all the question in Chinese only");
            auto response = chatGpt.askChatGPT("user"); //讓使用者問問題並記錄使用者的回答道response

            std::string bot_response_string=""; //紀錄此輪chatgpt的回答

            //Iterate all answers and adding it to answer
            for(const auto& choice:response.choices){
                std::cout<<choice.message.content;
            }
            
        }
    }catch(OpenAI::Error& e){
        //JSON error returned by the server
        std::cout<<e.what();
    }
}