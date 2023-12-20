#include <string>
#include "Types.h"
#include <vector>
namespace OpenAI {
    struct Message{
        std::string role;
        std::string content;

        Message(std::string role, std::string content); //constructor
    };

    class ChatGPT {
        public:
            explicit ChatGPT(const std::string& token); //constructor
            OpenAI::ChatCompletion askChatGPT(const std::string& role, bool isEnding); //using prompt
            void addPrompt(const Message& prompt); //把新的prompt加入this->prompts中

        private:
            std::vector< Message > prompts; //儲存所有給需要prompt給ChatGPT的資料
            std::string m_token;
            std::string m_link;
            std::string formatPrompt(const Message& prompt, bool isLast, bool isEnding);
            std::string promptsToStringContent(bool isEnding);
    };
}
