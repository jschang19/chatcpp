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
            explicit ChatGPT(const std::string& token);
            OpenAI::ChatCompletion askChatGPT(const std::string& role, bool isEnding);
            std::vector< Message > prompts;
            void addPrompt(const Message& prompt);

        private:
            std::string m_token;
            std::string m_link;
            std::string formatPrompt(const Message& prompt, bool isLast, bool isEnding);
            std::string promptsToStringContent(bool isEnding);
    };
}
