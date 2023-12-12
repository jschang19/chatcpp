#include <iostream>
#include <string>
#include "ChatGPT/include/ChatGPT.h"
#include "ChatGPT/include/Error.h"

int main(int args,char** argv){
    //API token as argument
    // check if argument is passed
    if(args<2){
        std::cout<<"Please provide the API token as argument"<<std::endl;
        return 0;
    }
    OpenAI::ChatGPT chatGpt{argv[1]};
    try {
        // get user input
        std::string userInput;
        std::cout<<"Enter your message: ";
        std::getline(std::cin,userInput);
        auto response = chatGpt.askChatGPT("user",userInput);
        //Iterate all answers
        for(const auto& choice:response.choices){
            std::cout<<choice.message.content;
        }
    }catch(OpenAI::Error& e){
        //JSON error returned by the server
        std::cout<<e.what();
    }
}