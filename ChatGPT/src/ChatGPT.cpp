#include "../include/ChatGPT.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector> 
#include "../include/Error.h"
#include <string>

OpenAI::Message::Message(std::string role, std::string content) : role(role), content(content){}

OpenAI::ChatGPT::ChatGPT(const std::string& token):m_link{"https://api.openai.com/v1/chat/completions"} {
    if(token.empty()){
        throw OpenAI::Error{"API token is empty"};
    }
    m_token = token;
}

OpenAI::ChatCompletion OpenAI::ChatGPT::askChatGPT(const std::string& role) {
    std :: string prompt_message; //在json中回傳的message
    prompt_message= this->PromptsToStringContent();

    auto json="{\n"
                    "  \"model\": \"gpt-3.5-turbo\",\n"
                    "  \"messages\": ["+ prompt_message +"]\n"
                    "}";

    auto response = cpr::Post(cpr::Url{m_link},cpr::Body{json},cpr::Bearer({m_token}),cpr::Header{{"Content-Type","application/json"}}).text;
    OpenAI::ChatCompletion chatCompletion;
    nlohmann::json j;
    try {
        j = nlohmann::json::parse(response);
    }catch(std::exception& e){
        std::cerr<<"Error: "+response;
    }
    if(!j.contains("error")) {
        from_json(j, chatCompletion);
    }else{
        throw OpenAI::Error{j.dump()};
    }

    return chatCompletion;
}

std::string OpenAI::ChatGPT::askWhisper(const std::string &audio_path) {
    std::string url = "https://api.openai.com/v1/audio/transcriptions";
    std::string model_name = "whisper-1";
    std::string auth_token = "Bearer " + std::string(m_token);

    cpr::Multipart multipart_data = {
            {"model", model_name},
            {"file", cpr::File{audio_path}}
    };

    auto r = cpr::Post(cpr::Url{url},
                                cpr::Header{{"Authorization", auth_token},
                                            {"Content-Type", "multipart/form-data"}},
                                multipart_data).text;
    nlohmann::json j;
    try {
        j = nlohmann::json::parse(r);
    }catch(std::exception& e){
        std::cerr<<"Error: "+r;
    }
    if(j.contains("error")) {
        throw OpenAI::Error{j.dump()};
    }
    return j["text"];
}

void OpenAI::ChatGPT::Add_prompt(const std::string& new_role ,const std::string& new_content){
    Message new_prompt(new_role, new_content);
    this->prompts.push_back(new_prompt);
}

std::string OpenAI::ChatGPT::PromptsToStringContent(){
    std :: string return_string="";
    for(int i=0; i<this->prompts.size(); i++){
        return_string += " {\"role\": \"" + this->prompts[i].role + "\" , \"content\": \"" + this->prompts[i].content + "\" }";
        if (i != this->prompts.size()-1 ){
            return_string+=",\n";
        }else{
            return_string+="\n";
        }
    }


    return return_string;
}
