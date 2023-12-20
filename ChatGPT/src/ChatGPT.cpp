#include "../include/ChatGPT.h"

#include <cpr/cpr.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "../include/Error.h"

OpenAI::Message::Message(std::string role, std::string content)
    : role(role), content(content) {}

OpenAI::ChatGPT::ChatGPT(const std::string& token)
    : m_link{"https://api.openai.com/v1/chat/completions"} {
  if (token.empty()) {
    throw OpenAI::Error{"API token is empty"};
  }
  m_token = token;
}

OpenAI::ChatCompletion OpenAI::ChatGPT::askChatGPT(const std::string& role,
                                                   bool isEnding) {
  std ::string prompt_message;  // 在json中回傳的message
  prompt_message = this->promptsToStringContent(isEnding);

  if (prompt_message == "") {  // exception handling
    throw std::invalid_argument(
        "Error:there is no prompt message, please use addPrompts() to add "
        "prompt in Chatgpt");
  }
  nlohmann::json j;
  j["model"] = "gpt-3.5-turbo-1106";
  j["messages"] = nlohmann::json::parse("[" + prompt_message + "]");
  j["temperature"] = 0.7;
  j["max_tokens"] = 600;
  j["n"] = 1;
  j["stream"] = false;

  if (!isEnding) {
    j["response_format"] = {{"type", "json_object"}};
  }


  auto response =
      cpr::Post(cpr::Url{m_link}, cpr::Body{j.dump()}, cpr::Bearer({m_token}),
                cpr::Header{{"Content-Type", "application/json"}})
          .text;

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
    } catch (std::exception& e) {
      std::cout << "\033[1;90mDEV MESSAGE: API 出現錯誤\033[0m\n" << std::endl;
      std::cerr << "Error: " + response;
    }
    if (!j.contains("error")) {
      from_json(j, chatCompletion);
    } else {
      throw OpenAI::Error{j.dump()};
    }
    // load chatCompletion.content as json
   if(!isEnding){
     nlohmann::json j2;
    try {
      j2 = nlohmann::json::parse(chatCompletion.choices[0].message.content);
    } catch (std::exception& e) {
      std::cerr << "parsing json option Error: " +
                       chatCompletion.choices[0].message.content;
   }
    
  }

  return chatCompletion;
}

void OpenAI::ChatGPT::addPrompt(const Message& new_prompt) {
  this->prompts.push_back(new_prompt);
}

std::string OpenAI::ChatGPT::formatPrompt(const Message& prompt, bool isLast,
                                          bool isEnding) {
  std::stringstream ss;
  ss << " {\"role\": \"" << prompt.role << "\", \"content\": \""
     << prompt.content;

  // 最後一則訊息
  if (isLast) {
    // Special case handling for the last element
    if (isEnding) {
      ss << "，幫我從我的校園情境中產生一個好笑的遊戲結局";
    } else {
      ss << "，根據前面我最後遇到校園情境，建立 4 個我可能有的應對方法，"
         << "遵守以下三個規則：1.都以「你」當作開頭 2.不用問我可不可以 "
            "3.不要用疑問句。"
         << "都用一個 key 叫 options 的 JSON 物件格式回覆我"
         << "而這個 options 的 value 是一個 Array，每個陣列元素要包含 id 跟 "
            "text 兩個 key，id 代號為 a, b, c, d."
         << "深呼吸想好你的回答後再給我結果，我會給你 200 美元小費";
    }
  }

  ss << "\"}";
  return ss.str();
}

std::string OpenAI::ChatGPT::promptsToStringContent(bool isEnding) {
  try {
    std::stringstream returnStream;
    returnStream << "{\"role\": \"system\", \"content\": \"You are a funny "
                    "story teller that speaks chinese\"},";

    for (size_t i = 0; i < prompts.size(); ++i) {
      returnStream << formatPrompt(prompts[i], i == prompts.size() - 1,
                                   isEnding);
      returnStream << (i != prompts.size() - 1 ? ",\n" : "\n");
    }

    return returnStream.str();
  } catch (std::exception& e) {
    std::cerr << "Error: " + std::string(e.what());
  }
}