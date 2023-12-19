#include "../include/ChatGPT.h"
#include "../include/Error.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <vector> 
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

    if (prompt_message==""){ //exception handling
        throw std::invalid_argument("Error:there is no prompt message, please use Add_prompts() to add prompt in Chatgpt");
    }

    nlohmann::json j;
    j["model"] = "gpt-3.5-turbo-1106";
    j["messages"] = nlohmann::json::parse("[" + prompt_message + "]");
    j["response_format"] = {{"type", "json_object"}};
    j["temperature"] = 0.8;
    j["max_tokens"] = 1000;
    j["n"] = 1;

    auto response = cpr::Post(cpr::Url{m_link}, 
                              cpr::Body{j.dump()}, 
                              cpr::Bearer({m_token}), 
                              cpr::Header{{"Content-Type", "application/json"}}).text;

    OpenAI::ChatCompletion chatCompletion;
    try {
        nlohmann::json j_response = nlohmann::json::parse(response);
        if (!j_response.contains("error")) {
            from_json(j_response, chatCompletion);
        } else {
            throw OpenAI::Error{j_response.dump()};
        }
    } catch (std::exception& e) {
        std::cout << "\033[1;90mDEV MESSAGE: API 出現錯誤\033[0m\n" << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
    }
    try {
        j = nlohmann::json::parse(response);
    }catch(std::exception& e){
        std::cout << "\033[1;90mDEV MESSAGE: API 出現錯誤\033[0m\n" << std::endl;
        std::cerr<<"Error: "+response;
    }
    if(!j.contains("error")) {
        from_json(j, chatCompletion);
    }else{
        throw OpenAI::Error{j.dump()};
    }
    // load chatCompletion.content as json
    nlohmann::json j2;
    try {
        j2 = nlohmann::json::parse(chatCompletion.choices[0].message.content);
    }catch(std::exception& e){
        std::cerr<<"parsing j2 Error: "+chatCompletion.choices[0].message.content;
    }
    // add chatCompletion.choices[0].message.content into prompts
    // this->Add_prompt(OpenAI::Message("assistant", j2["options"]));
    return chatCompletion;
}

std::string OpenAI::ChatGPT::askWhiasper(const std::string& audio_path) {
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

void OpenAI::ChatGPT::Add_prompt(const Message& new_prompt){
    this->prompts.push_back(new_prompt);
}

std::string OpenAI::ChatGPT::PromptsToStringContent(){
    std :: string return_string="{\"role\": \"system\", \"content\": \"You are a text game system that generates options to make young college players choos. you must produce content in Traditional Chinese.\"},";
    for(int i=0; i<this->prompts.size(); i++){
        return_string += " {\"role\": \"" + this->prompts[i].role + "\" , \"content\": \"" + this->prompts[i].content;
        if (i == this->prompts.size()-1){
            return_string += "，幫我想 4 個搞笑且具有創意的遊戲情境讓我選擇，並確保選項都跟之前我的選擇行動有關，規則：1.都以「你」當作開頭,2.不要加「可以」這個字,3.都以句號結尾，不要用疑問句。都用一個 key 叫 options 的 JSON 物件格式回覆我，而這個 options 的 value 是一個 Array，每個陣列元素要包含 id 跟 text 兩個 key，id 代號為 a, b, c, d";
        }
        return_string += "\"}";
        //處理換行符號
        if (i != this->prompts.size()-1 ){
            return_string+=",\n";
        }else{
            return_string+="\n";
        }
    }
    return return_string;
}
