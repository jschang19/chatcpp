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
            OpenAI::ChatCompletion askChatGPT(const std::string& role);
            std::string askWhisper(const std::string& audio_path);
            std::vector< Message > prompts;
            void Add_prompt(const Message& prompt);

        private:
            std::string m_token;
            std::string m_link;

            std::string PromptsToStringContent();
    };
}
